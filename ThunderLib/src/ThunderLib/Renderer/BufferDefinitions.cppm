export module ThunderLib.BufferDefinitions;

import std;
import ThunderLib.Base;
import ThunderLib.Logger;

export namespace ThunderLib
{
	/// <summary>
	/// Datatypes of shader related variables
	/// </summary>
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	/// <summary>
	/// Returns the size in bytes of the ShaderDataType
	/// </summary>
	/// <param name="type">A valid ShaderDataType</param>
	/// <returns>Size in bytes of the provided type</returns>
	uint32_t GetShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Mat3: return 4 * 3 * 3;
		case ShaderDataType::Mat4: return 4 * 4 * 4;
		case ShaderDataType::Int: return 4;
		case ShaderDataType::Int2: return 4 * 2;
		case ShaderDataType::Int3: return 4 * 3;
		case ShaderDataType::Int4: return 4 * 4;
		case ShaderDataType::Bool: return 1;
		default: break;
		}

		Logger::Warning("Unknown ShaderDataType! Cannot get size.");
		return 0;
	}

	/// <summary>
	/// Represents an element of a buffer's vertex
	/// </summary>
	struct BufferElement
	{
		ShaderDataType type; // Type of data
		uint32_t size;       // Size in bytes of data
		size_t offset = 0;   // Offset in bytes compared to the Vertex's base address
		bool normalized;     // If vector, is it normalized?
		std::string name;    // Name of element

		BufferElement() = default;

		BufferElement(const std::string& name, ShaderDataType type, bool normalized = false)
			: type(type), size(GetShaderDataTypeSize(type)), normalized(normalized), name(name)
		{}
	};

	/// <summary>
	/// A description of how a Buffer's Vertex are laid out in memory
	/// </summary>
	class BufferLayout
	{
	private:
		std::vector<BufferElement> elements_;
		uint32_t stride_ = 0;

	public:
		BufferLayout() = default;

		/// <summary>
		/// Creates a BufferLayout from the provided list of BufferElements
		/// </summary>
		/// <param name="elements">An array list of BufferElements</param>
		BufferLayout(std::initializer_list<BufferElement> elements)
			:elements_(elements)
		{
			CalculateOffsetsAndStride();
		}

		/// <summary>
		/// Returns the stride of a Vertex with this BufferLayout
		/// </summary>
		/// <returns>Stride in bytes of a vertex with this layout</returns>
		uint32_t GetStride() const { return stride_; }

		/// <summary>
		/// Returns the vector of BufferElements of this layout
		/// </summary>
		/// <returns>A vector of BufferElements</returns>
		const std::vector<BufferElement>& GetElements() const { return elements_; }

	private:
		
		// Calculates the offset of each BufferElement and the total vertex stride
		void CalculateOffsetsAndStride();
	};
}

namespace ThunderLib
{

	void BufferLayout::CalculateOffsetsAndStride()
	{
		size_t offset = 0;
		stride_ = 0;

		for (BufferElement& element : elements_) {
			element.offset = offset;
			offset += element.size;
			stride_ += element.size;
		}
	}
}