// #pragma once
//
// #include <vulkan/vulkan_raii.hpp>
//
// #include <renderlib/RenderingInfo.hpp>
// #include <renderlib/resources/Format.hpp>
// #include <renderlib/resources/Pipeline.hpp>
// #include <renderlib/resources/Swapchain.hpp>
//
// // Enum conversions
// constexpr vk::Format ToVulkanFormatFromVertex(VertexFormat format)
// {
//     switch (format)
//     {
//     case VertexFormat::Float32:
//         return vk::Format::eR32Sfloat;
//     case VertexFormat::Float32x2:
//         return vk::Format::eR32G32Sfloat;
//     case VertexFormat::Float32x3:
//         return vk::Format::eR32G32B32Sfloat;
//     case VertexFormat::Float32x4:
//         return vk::Format::eR32G32B32A32Sfloat;
//     case VertexFormat::UInt8x4Norm:
//         return vk::Format::eR8G8B8A8Unorm;
//     default:
//         return vk::Format::eUndefined;
//     }
// }
//
// constexpr vk::Format ToVulkanFormat(Format format)
// {
//     switch (format)
//     {
//     case Format::Undefined:
//         return vk::Format::eUndefined;
//
//     // 8-bit per channel color
//     case Format::R8Unorm:
//         return vk::Format::eR8Unorm;
//     case Format::RG8Unorm:
//         return vk::Format::eR8G8Unorm;
//     case Format::RGBA8Unorm:
//         return vk::Format::eR8G8B8A8Unorm;
//     case Format::RGBA8Srgb:
//         return vk::Format::eR8G8B8A8Srgb;
//     case Format::BGRA8Unorm:
//         return vk::Format::eB8G8R8A8Unorm;
//     case Format::BGRA8Srgb:
//         return vk::Format::eB8G8R8A8Srgb;
//
//     // Float color
//     case Format::R16Float:
//         return vk::Format::eR16Sfloat;
//     case Format::RG16Float:
//         return vk::Format::eR16G16Sfloat;
//     case Format::RGBA16Float:
//         return vk::Format::eR16G16B16A16Sfloat;
//     case Format::R32Float:
//         return vk::Format::eR32Sfloat;
//     case Format::RGBA32Float:
//         return vk::Format::eR32G32B32A32Sfloat;
//
//     // Depth / depth-stencil
//     case Format::D16Unorm:
//         return vk::Format::eD16Unorm;
//     case Format::D32Float:
//         return vk::Format::eD32Sfloat;
//     case Format::D24UnormS8Uint:
//         return vk::Format::eD24UnormS8Uint;
//     case Format::D32FloatS8Uint:
//         return vk::Format::eD32SfloatS8Uint;
//
//     default:
//         return vk::Format::eUndefined;
//     }
// }
//
// constexpr Format FromVulkanFormat(vk::Format format)
// {
//     switch (format)
//     {
//     case vk::Format::eUndefined:
//         return Format::Undefined;
//
//     case vk::Format::eR8Unorm:
//         return Format::R8Unorm;
//     case vk::Format::eR8G8Unorm:
//         return Format::RG8Unorm;
//     case vk::Format::eR8G8B8A8Unorm:
//         return Format::RGBA8Unorm;
//     case vk::Format::eR8G8B8A8Srgb:
//         return Format::RGBA8Srgb;
//     case vk::Format::eB8G8R8A8Unorm:
//         return Format::BGRA8Unorm;
//     case vk::Format::eB8G8R8A8Srgb:
//         return Format::BGRA8Srgb;
//
//     case vk::Format::eR16Sfloat:
//         return Format::R16Float;
//     case vk::Format::eR16G16Sfloat:
//         return Format::RG16Float;
//     case vk::Format::eR16G16B16A16Sfloat:
//         return Format::RGBA16Float;
//     case vk::Format::eR32Sfloat:
//         return Format::R32Float;
//     case vk::Format::eR32G32B32A32Sfloat:
//         return Format::RGBA32Float;
//
//     case vk::Format::eD16Unorm:
//         return Format::D16Unorm;
//     case vk::Format::eD32Sfloat:
//         return Format::D32Float;
//     case vk::Format::eD24UnormS8Uint:
//         return Format::D24UnormS8Uint;
//     case vk::Format::eD32SfloatS8Uint:
//         return Format::D32FloatS8Uint;
//
//     default:
//         return Format::Undefined;
//     }
// }
//
// constexpr vk::PrimitiveTopology ToVulkanPrimitiveTopology(PrimitiveTopology topology)
// {
//     switch (topology)
//     {
//     case PrimitiveTopology::LineList:
//         return vk::PrimitiveTopology::eLineList;
//     case PrimitiveTopology::PointList:
//         return vk::PrimitiveTopology::ePointList;
//     case PrimitiveTopology::TriangleList:
//         return vk::PrimitiveTopology::eTriangleList;
//     default:
//         return vk::PrimitiveTopology::eTriangleList;
//     }
// }
//
// constexpr vk::BufferUsageFlags ToVulkanBufferUsageType(BufferType type)
// {
//     switch (type)
//     {
//     case BufferType::Index:
//         return vk::BufferUsageFlagBits::eIndexBuffer;
//     case BufferType::Vertex:
//         return vk::BufferUsageFlagBits::eVertexBuffer;
//     case BufferType::Uniform:
//         return vk::BufferUsageFlagBits::eUniformBuffer;
//     case BufferType::Storage:
//         return vk::BufferUsageFlagBits::eStorageBuffer;
//     default:
//         return vk::BufferUsageFlagBits::eVertexBuffer;
//     }
// }
//
// constexpr vk::CullModeFlags ToVulkanCullMode(CullMode cullMode)
// {
//     switch (cullMode)
//     {
//     case CullMode::Back:
//         return vk::CullModeFlagBits::eBack;
//     case CullMode::Front:
//         return vk::CullModeFlagBits::eFront;
//     case CullMode::None:
//         return vk::CullModeFlagBits::eNone;
//     default:
//         return vk::CullModeFlagBits::eNone;
//     }
// }
//
// constexpr vk::CompareOp ToVulkanCompareOp(CompareOp compareOp)
// {
//     switch (compareOp)
//     {
//     case CompareOp::Always:
//         return vk::CompareOp::eAlways;
//     case CompareOp::Equal:
//         return vk::CompareOp::eEqual;
//     case CompareOp::Greater:
//         return vk::CompareOp::eGreater;
//     case CompareOp::Less:
//         return vk::CompareOp::eLess;
//     default:
//         return vk::CompareOp::eAlways;
//     }
// }
//
// constexpr vk::DescriptorType ToVulkanDescriptorType(DescriptorType type)
// {
//     switch (type)
//     {
//     case DescriptorType::UniformBuffer:
//         return vk::DescriptorType::eUniformBuffer;
//     case DescriptorType::StorageBuffer:
//         return vk::DescriptorType::eStorageBuffer;
//     case DescriptorType::SampledTexture:
//         return vk::DescriptorType::eCombinedImageSampler;
//     case DescriptorType::StorageTexture:
//         return vk::DescriptorType::eStorageImage;
//     case DescriptorType::Sampler:
//         return vk::DescriptorType::eSampler;
//     default:
//         return vk::DescriptorType::eUniformBuffer;
//     }
// }
//
// constexpr vk::ShaderStageFlags ToVulkanShaderStageFlags(ShaderStageFlags flags)
// {
//     vk::ShaderStageFlags result{};
//
//     if (HasFlag(flags, ShaderStageFlags::Vertex))
//         result |= vk::ShaderStageFlagBits::eVertex;
//
//     if (HasFlag(flags, ShaderStageFlags::Fragment))
//         result |= vk::ShaderStageFlagBits::eFragment;
//
//     if (HasFlag(flags, ShaderStageFlags::Compute))
//         result |= vk::ShaderStageFlagBits::eCompute;
//
//     return result;
// }
//
// constexpr vk::Filter ToVulkanFilter(FilterMode mode)
// {
//     switch (mode)
//     {
//     case FilterMode::Nearest:
//         return vk::Filter::eNearest;
//     case FilterMode::Linear:
//         return vk::Filter::eLinear;
//     default:
//         return vk::Filter::eLinear;
//     }
// }
//
// constexpr vk::SamplerMipmapMode ToVulkanMipmapMode(FilterMode mode)
// {
//     switch (mode)
//     {
//     case FilterMode::Nearest:
//         return vk::SamplerMipmapMode::eNearest;
//     case FilterMode::Linear:
//         return vk::SamplerMipmapMode::eLinear;
//     default:
//         return vk::SamplerMipmapMode::eLinear;
//     }
// }
//
// constexpr vk::SamplerAddressMode ToVulkanAddressMode(AddressMode mode)
// {
//     switch (mode)
//     {
//     case AddressMode::Repeat:
//         return vk::SamplerAddressMode::eRepeat;
//     case AddressMode::ClampToEdge:
//         return vk::SamplerAddressMode::eClampToEdge;
//     case AddressMode::ClampToBorder:
//         return vk::SamplerAddressMode::eClampToBorder;
//     case AddressMode::MirroredRepeat:
//         return vk::SamplerAddressMode::eMirroredRepeat;
//     default:
//         return vk::SamplerAddressMode::eRepeat;
//     }
// }
//
// constexpr vk::ImageType ToVulkanImageType(TextureType type)
// {
//     switch (type)
//     {
//     case TextureType::Texture1D:
//         return vk::ImageType::e1D;
//     case TextureType::Texture2D:
//         return vk::ImageType::e2D;
//     case TextureType::Texture3D:
//         return vk::ImageType::e3D;
//     case TextureType::TextureCube:
//         return vk::ImageType::e2D;
//     default:
//         return vk::ImageType::e2D;
//     }
// }
//
// constexpr vk::ImageViewType ToVulkanImageViewType(TextureType type)
// {
//     switch (type)
//     {
//     case TextureType::Texture1D:
//         return vk::ImageViewType::e1D;
//     case TextureType::Texture2D:
//         return vk::ImageViewType::e2D;
//     case TextureType::Texture3D:
//         return vk::ImageViewType::e3D;
//     case TextureType::TextureCube:
//         return vk::ImageViewType::eCube;
//     default:
//         return vk::ImageViewType::e2D;
//     }
// }
//
// constexpr vk::ImageUsageFlags ToVulkanImageUsageFlags(TextureUsage usage)
// {
//     vk::ImageUsageFlags flags{};
//
//     if (hasFlag(usage, TextureUsage::Sampled))
//         flags |= vk::ImageUsageFlagBits::eSampled;
//
//     if (hasFlag(usage, TextureUsage::RenderTarget))
//         flags |= vk::ImageUsageFlagBits::eColorAttachment;
//
//     if (hasFlag(usage, TextureUsage::DepthStencil))
//         flags |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
//
//     if (hasFlag(usage, TextureUsage::Storage))
//         flags |= vk::ImageUsageFlagBits::eStorage;
//
//     if (hasFlag(usage, TextureUsage::TransferSrc))
//         flags |= vk::ImageUsageFlagBits::eTransferSrc;
//
//     if (hasFlag(usage, TextureUsage::TransferDst))
//         flags |= vk::ImageUsageFlagBits::eTransferDst;
//
//     return flags;
// }
//
// constexpr vk::ImageAspectFlags ToVulkanImageAspectFlags(Format format)
// {
//     if (IsDepthFormat(format))
//     {
//         return HasStencilComponent(format) ? (vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil)
//                                            : vk::ImageAspectFlagBits::eDepth;
//     }
//
//     return vk::ImageAspectFlagBits::eColor;
// }
//
// constexpr vk::PresentModeKHR ToVulkanPresentMode(PresentMode mode)
// {
//     switch (mode)
//     {
//     case PresentMode::Immediate:
//         return vk::PresentModeKHR::eImmediate;
//     case PresentMode::Fifo:
//         return vk::PresentModeKHR::eFifo;
//     case PresentMode::Mailbox:
//         return vk::PresentModeKHR::eMailbox;
//     default:
//         return vk::PresentModeKHR::eFifo;
//     }
// }
//
// constexpr vk::AttachmentLoadOp ToVulkanLoadOp(LoadOp op)
// {
//     switch (op)
//     {
//     case LoadOp::Load:
//         return vk::AttachmentLoadOp::eLoad;
//     case LoadOp::Clear:
//         return vk::AttachmentLoadOp::eClear;
//     case LoadOp::DontCare:
//         return vk::AttachmentLoadOp::eDontCare;
//     default:
//         return vk::AttachmentLoadOp::eDontCare;
//     }
// }
//
// constexpr vk::AttachmentStoreOp ToVulkanStoreOp(StoreOp op)
// {
//     switch (op)
//     {
//     case StoreOp::Store:
//         return vk::AttachmentStoreOp::eStore;
//     case StoreOp::DontCare:
//         return vk::AttachmentStoreOp::eDontCare;
//     default:
//         return vk::AttachmentStoreOp::eDontCare;
//     }
// }
//
// constexpr vk::IndexType ToVulkanIndexType(IndexType type)
// {
//     switch (type)
//     {
//     case IndexType::UInt16:
//         return vk::IndexType::eUint16;
//     case IndexType::UInt32:
//         return vk::IndexType::eUint32;
//     default:
//         return vk::IndexType::eUint16;
//     }
// }
//
// inline std::vector<vk::VertexInputAttributeDescription> ToVulkanAttributeDescriptions(const VertexLayout &layout,
//                                                                                       uint32_t binding = 0)
// {
//     std::vector<vk::VertexInputAttributeDescription> attributeDescs;
//     attributeDescs.reserve(layout.attributes.size());
//
//     for (const auto &attr : layout.attributes)
//     {
//         vk::VertexInputAttributeDescription attributeDesc{};
//         attributeDesc.location = attr.location;
//         attributeDesc.binding = binding;
//         attributeDesc.format = ToVulkanFormatFromVertex(attr.format);
//         attributeDesc.offset = attr.offset;
//
//         attributeDescs.push_back(attributeDesc);
//     }
//
//     return attributeDescs;
// }
//
// inline vk::VertexInputBindingDescription ToVulkanBindingDescription(const VertexLayout &layout, uint32_t binding = 0)
// {
//     vk::VertexInputBindingDescription bindingDesc{};
//     bindingDesc.binding = binding;
//     bindingDesc.stride = layout.stride;
//     bindingDesc.inputRate = vk::VertexInputRate::eVertex;
//
//     return bindingDesc;
// }
//
// inline vk::PipelineColorBlendAttachmentState ToVulkanColorBlendAttachment(BlendMode mode)
// {
//     vk::PipelineColorBlendAttachmentState state{};
//     state.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
//                             vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
//
//     switch (mode)
//     {
//     case BlendMode::Opaque:
//         state.setBlendEnable(vk::False);
//         break;
//
//     case BlendMode::AlphaBlend:
//         state.setBlendEnable(vk::True)
//             .setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
//             .setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
//             .setColorBlendOp(vk::BlendOp::eAdd)
//             .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
//             .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
//             .setAlphaBlendOp(vk::BlendOp::eAdd);
//         break;
//
//     case BlendMode::Additive:
//         state.setBlendEnable(vk::True)
//             .setSrcColorBlendFactor(vk::BlendFactor::eOne)
//             .setDstColorBlendFactor(vk::BlendFactor::eOne)
//             .setColorBlendOp(vk::BlendOp::eAdd)
//             .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
//             .setDstAlphaBlendFactor(vk::BlendFactor::eOne)
//             .setAlphaBlendOp(vk::BlendOp::eAdd);
//         break;
//
//     default:
//         state.setBlendEnable(vk::False);
//         break;
//     }
//
//     return state;
// }
//
// inline vk::DescriptorSetLayoutBinding ToVulkanDescriptorSetLayoutBinding(const DescriptorBindingCreateInfo &binding)
// {
//     vk::DescriptorSetLayoutBinding vkBinding{};
//
//     vkBinding.setBinding(binding.binding)
//         .setDescriptorType(ToVulkanDescriptorType(binding.type))
//         .setDescriptorCount(binding.count)
//         .setStageFlags(ToVulkanShaderStageFlags(binding.stageFlags));
//
//     return vkBinding;
// }
//
// inline std::vector<vk::DescriptorSetLayoutBinding> ToVulkanDescriptorSetLayoutBindings(
//     const std::vector<DescriptorBindingCreateInfo> &bindings)
// {
//     std::vector<vk::DescriptorSetLayoutBinding> vkBindings;
//     vkBindings.reserve(bindings.size());
//
//     for (const auto &binding : bindings)
//     {
//         vkBindings.push_back(ToVulkanDescriptorSetLayoutBinding(binding));
//     }
//
//     return vkBindings;
// }
