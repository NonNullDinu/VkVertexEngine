# Creating a vertex structure
This guide explains how to create a vertex structure that you can use
with `Vertex::VertexArray::Create(std::vector<T>,std::vector<uint32_t>)` to create a vertex array.

First, let's declare an empty structure:
```c++
struct VertexData {
    
};
```

Then, as we all know, the actual data needs to be put in some vertex buffers, that this particular `VertexArray::Create`
handles by itself, as long as we tell it how.
There are three templates structures declared in `GL/Vulkan/VulkanVertexShaderInputHelper.h` that help a lot:

## `VxDataBuffer`
```c++
template <VxDataBufferType type, VxSize Binding, VxSize Offset = 0>
struct VxDataBuffer;
```
- The `VxDataBufferType` is an enum that can the type of buffer(`STATIC` or `DYNAMIC`)
- The `Binding` is the one used for the vk structures needed to create the shader pipeline 
that consumes this kind of data. If you do not know what this means, start off with 0 and
increment each time you add a new buffer and that should be enough.
- The `Offset` is how much data there is between this and the previous `VxDataBuffer`.
The offset of the first one should be `0`(not necessarily the first member of the struct; more about this [here](#GetVertexArrayLayout-Implementation)).

## `VertexField`
```c++
template <typename T, int Offset, int Location, VkFormat Format = VertexShaderInputUtils::DeduceFormat<T>()>
struct VertexField;
```
- The `T` is the parent type, so the `VertexField` gets `T`'s methods for easier use(it also tries to implement all
possible constructors; this may fail if the `T` constructor you're trying to call uses templates and they cannot be
deduced from the arguments. If you know of a workaround please open an issue on [this repo](https://github.com/NonNullDinu/VkVertexEngine)).
- The `Offset` this time is the offset _within the `VxDatBuffer`_ that this field has, as we'll see [here](#Filling-the-struct).
- The `Location` is the `layout (location = ...) in` from the vertex shader.
- The `Format` is the `VkFormat` that this field uses; The `DeduceFormat` function can handle by itself the basic types like:
  - `float`
  - `glm::vec1`
  - `glm::vec2`
  - `glm::vec3`
  - `glm::vec4`
  - `double`
  - `glm::dvec1`
  - `glm::dvec2`
  - `glm::dvec3`
  - `glm::dvec4`


## `VulkanVertexShaderInputs`
The declaration is as follows
```c++
template <VulkanVertexShaderInputsParam... Args>
struct VulkanVertexShaderInputs;
```
Now this is where all the magic happens. This type declares two `constexpr static`
methods that we will use(see [here](#ShaderInput-calls)).

# Filling the struct
Let's start where we left off. So we have our empty struct:
```c++
struct VertexData {
    
};
```

Now let's say our vertex shader has those inputs:
```glsl
layout(location = 0) in vec3 postion;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec4 some_dynamic_data;
layout(location = 3) in vec3 normal;
layout(location = 4) in vec3 some_other_dynamic_data;
```
And that we want our buffers to have the data like this:
```
Binding 0 | position | tex_coord | normal |
Binding 1 | somd_dynamic_data | some_other_dynamic_data |
```

Let's first declare the `VxDataBuffer`s.

The first one like:
```c++
struct StaticDataType : public Vertex::VxDataBuffer<STATIC, 0>
{
} StaticData;
```

And the second one like:
```c++
struct DynamicDataType : public Vertex::VxDataBuffer<DYNAMIC, 1>
{
} DynamicData;
```
Remember, the second template argument for the `VxDataBuffer` is the binding.

Now let's move on to adding the fields.

```c++
struct StaticDataType : public Vertex::VxDataBuffer<STATIC, 0>
{
    Vertex::VertexField<glm::vec3, 0, 0> position; // offset = 0, location = 0
    Vertex::VertexField<glm::vec2, sizeof(position), 1> tex_coord; // offset = sizeof(position), location = 1
    Vertex::VertexField<glm::vec3, sizeof(position)+sizeof(tex_coord), 3> normal; // offset = sizeof(position) + sizeof(tex_coord), location = 3
} StaticData;
```

```c++
struct DynamicDataType : public Vertex::VxDataBuffer<DYNAMIC, 1>
{
    Vertex::VertexField<glm::vec4, 0, 2> some_dynamic_data; // offset = 0, location = 2
    Vertex::VertexField<glm::vec3, sizeof(some_dynamic_data), 4> some_other_dynamic_data; // offset = sizeof(some_dynamic_data), location = 4
};
```

Now we should add the `VulkanVertexShaderInputs`
```c++
using ShaderInputs = Vertex::VulkanVertexShaderInputs<>;
```

The `VulkanVertexShaderInputs` expects to find arguments in this order:
`<VxDataBuffer 0, VertexField 0, VertexField 1, VertexField 2, ..., VxDataBuffer 1, VertexField 0, VertexField 2, ..., VxDataBuffer 3, ...>`
Each `VxDataBuffer` has immediately after it the VertexFields that are part of it. In our case:
```c++
using ShaderInputs = Vertex::VulkanVertexShaderInputs
<
    decltype(StaticData), decltype(StaticData.position), decltype(StaticData.tex_coord), decltype(StaticData.normal),
    decltype(DynamicData), decltype(DynamicData.some_dynamic_data), decltype(DynamicData.some_other_dynamic_data)
>;
```

### ShaderInput calls
Now with our `ShaderInputs` type set up, we can call the static functions easily. Now let us declare 2 static functions:
```c++
static constexpr auto GetAttributeDescriptions() 
{
}
static constexpr auto GetVertexArrayLayout()
{
}
```
Technically, only `GetVertexArrayLayout` is needed, but the `ShaderInputs` also helps us create the shader pipeline.
Let us start with `GetAttributeDescriptions`, which is really simple:
```c++
static constexpr auto GetAttributeDescriptions() 
{
    return ShaderInputs::GetAttributeDescriptions();
}
```
##### GetVertexArrayLayout Implementation
```c++
static constexpr auto GetVertexArrayLayout()
{
    return ShaderInputs::GetVertexArrayLayout<offsetof(VertexData, StaticData)>();
}
```
The template parameter that `GetVertexArrayLayout` needs is the offset of the first `VxDataBuffer`, so we will use `offsetof` to get it.

Now, if you followed this guide, you should have a structure like this:
```c++
struct VertexData {
    struct StaticDataType : public Vertex::VxDataBuffer<STATIC, 0>
    {
        Vertex::VertexField<glm::vec3, 0, 0> position;
        Vertex::VertexField<glm::vec2, sizeof(position), 1> tex_coord;
        Vertex::VertexField<glm::vec3, sizeof(position)+sizeof(tex_coord), 3> normal;
    } StaticData;

    struct DynamicDataType : public Vertex::VxDataBuffer<DYNAMIC, 1>
    {
        Vertex::VertexField<glm::vec4, 0, 2> some_dynamic_data;
        Vertex::VertexField<glm::vec3, sizeof(some_dynamic_data), 4> some_other_dynamic_data;
    };

    using ShaderInputs = Vertex::VulkanVertexShaderInputs
    <
        decltype(StaticData), decltype(StaticData.position), decltype(StaticData.tex_coord), decltype(StaticData.normal),
        decltype(DynamicData), decltype(DynamicData.some_dynamic_data), decltype(DynamicData.some_other_dynamic_data)
    >;

    static constexpr auto GetAttributeDescriptions()
    {
        return ShaderInputs::GetAttributeDescriptions();
    }

    static constexpr auto GetVertexArrayLayout()
    {
        return ShaderInputs::GetVertexArrayLayout<offsetof(VertexData, StaticData)>();
    }
}
```

Now, let's say you want to move the `normal` to another (static) buffer.
Then you need modify the struct like this:

```c++
struct VertexData {
    struct StaticDataType : public Vertex::VxDataBuffer<STATIC, 0>
    {
        Vertex::VertexField<glm::vec3, 0, 0> position;
        Vertex::VertexField<glm::vec2, sizeof(position), 1> tex_coord;
    } StaticData;

    ...

    struct StaticDataType2 : public Vertex::VxDataBuffer<STATIC, 2>
    {
        Vertex::VertexField<glm::vec3, 0, 3> normal;
    } StaticData2;

    using ShaderInputs = Vertex::VulkanVertexShaderInputs
    <
        decltype(StaticData), decltype(StaticData.position), decltype(StaticData.tex_coord),
        decltype(DynamicData), decltype(DynamicData.some_dynamic_data), decltype(DynamicData.some_other_dynamic_data),
        decltype(StaticData2), decltype(StaticData2.normal)
    >;

    ...
}
```

Now you can add constructors, and anything else that you might need.
You might need to account for padding if you use mixes of `vec` and `dvec`

## What do you use `GetAttributeDescriptions` for?
Good question. Remember we mentioned `ShaderInput` can also help us create the pipeline?
Well that is where it is used. Specifically, `GetAttributeDescriptions` returns the argument that
the 3rd parameter of `Vertex::Shader::Create` should receive.