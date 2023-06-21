#ifndef _MYENGINE_PRIMITIVES_H_
#define _MYENGINE_PRIMITIVES_H_

#include "engine/Defines.h"

namespace MYENGINE
{

	struct Manager;
	struct VertexBuffer;

	struct Primitives
	{
	public:
		Primitives() {};

		/**
		* Load a primitive VertexArray representing a plane into the cache.
		* 
		* @param _manager : the Manager instance to load into.
		**/
		static void CreatePlane(weak(Manager) _manager);

		/**
		* Load a primitive VertexArray representing a vertical plane into the cache.
		*
		* @param _manager : the Manager instance to load into.
		**/
		static void CreateVerticalPlane(weak(Manager) _manager);

		/**
		* Load a primitive VertexArray representing a vertical plane with its texture flipped into the cache.
		*
		* @param _manager : the Manager instance to load into.
		**/
		static void CreateTextureFlippedVerticalPlane(weak(Manager) _manager);


		/**
		* Load a primitive VertexArray representing a box into the cache.
		*
		* @param _manager : the Manager instance to load into.
		**/
		static void CreateBox(weak(Manager) _manager);

		/**
		* Load a primitive VertexArray representing a sky-box into the cache.
		*
		* @param _manager : the Manager instance to load into.
		**/
		static void CreateSkyBox(weak(Manager) _manager);


		/**
		* Load a primitive VertexArray representing a triangle into the cache.
		*
		* @param _manager : the Manager instance to load into.
		**/
		static void CreateTriangle(weak(Manager) _manager);

		/**
		* Load a primitive VertexArray representing an equalateral triangle into the cache.
		*
		* @param _manager : the Manager instance to load into.
		**/
		static void CreateEqTriangle(weak(Manager) _manager);

	private:
		static void CreateVertArray(weak(Manager) _manager, shared(VertexBuffer) _verts, shared(VertexBuffer) _texCoords, shared(VertexBuffer) _norms, const char* _name);
	};

} // namespace MYENGINE

#endif
