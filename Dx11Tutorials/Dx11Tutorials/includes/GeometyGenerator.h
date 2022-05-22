#include "Windows.h"

namespace orangelie {

	namespace Mesh {

		class GeometryGenerator {
		public:
			GeometryGenerator();
			GeometryGenerator(const GeometryGenerator&) = delete;
			GeometryGenerator& operator=(const GeometryGenerator&) = delete;
			virtual ~GeometryGenerator();

		};
	}
}
