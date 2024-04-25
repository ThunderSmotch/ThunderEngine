#include "Buffer_Base.h"

namespace ThunderEngine
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
