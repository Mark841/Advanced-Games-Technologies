#pragma once
#include "../../Common/Vector2.h"
#include "../CSC8503Common/CollisionDetection.h"
#include "Debug.h"
#include <list>
#include <functional>

namespace NCL {
	using namespace NCL::Maths;
	namespace CSC8503 {
		template<class T>
		class OctTree;

		template<class T>
		struct OctTreeEntry {
			Vector3 pos;
			Vector3 size;
			T object;

			OctTreeEntry(T obj, Vector3 pos, Vector3 size) {
				object		= obj;
				this->pos	= pos;
				this->size	= size;
			}
		};

		template<class T>
		class OctTreeNode	{
		public:
			typedef std::function<void(std::list<OctTreeEntry<T>>&)> OctTreeFunc;
		protected:
			friend class OctTree<T>;

			OctTreeNode() {}

			OctTreeNode(Vector3 pos, Vector3 size) {
				children		= nullptr;
				this->position	= pos;
				this->size		= size;
			}

			~OctTreeNode() {
				delete[] children;
			}

			void Insert(T& object, const Vector3& objectPos, const Vector3& objectSize, int depthLeft, int maxSize) {
				if (!CollisionDetection::AABBTest(objectPos, position, objectSize, size))
				{
					return;
				}
				if (children)
				{ // not a leaf node, just descend the tree
					for (int i = 0; i < 8; ++i)
					{
						children[i].Insert(object, objectPos, objectSize, depthLeft - 1, maxSize);
					}
				}
				else
				{ // currently a leaf node, can just expand
					contents.push_back(OctTreeEntry<T>(object, objectPos, objectSize));
					if ((int)contents.size() > maxSize && depthLeft > 0)
					{
						if (!children)
						{
							Split();
							// we need to reinsert the contents so far into the new subdivisions
							for (const auto& i : contents)
							{
								for (int j = 0; j < 8; ++j)
								{
									auto entry = i;
									children[j].Insert(entry.object, entry.pos, entry.size, depthLeft - 1, maxSize);
								}
							}
							contents.clear(); // contents have now been redistributed
						}
					}
				}
			}

			void Split() {
				Vector3 halfSize = size / 2.0f;
				children = new OctTreeNode<T>[8];
				children[0] = OctTreeNode<T>(position + Vector3(-halfSize.x, halfSize.y, halfSize.z), halfSize);
				children[1] = OctTreeNode<T>(position + Vector3(halfSize.x, halfSize.y, halfSize.z), halfSize);
				children[2] = OctTreeNode<T>(position + Vector3(-halfSize.x, -halfSize.y, halfSize.z), halfSize);
				children[3] = OctTreeNode<T>(position + Vector3(halfSize.x, -halfSize.y, halfSize.z), halfSize);
				children[4] = OctTreeNode<T>(position + Vector3(-halfSize.x, halfSize.y, -halfSize.z), halfSize);
				children[5] = OctTreeNode<T>(position + Vector3(halfSize.x, halfSize.y, -halfSize.z), halfSize);
				children[6] = OctTreeNode<T>(position + Vector3(-halfSize.x, -halfSize.y, -halfSize.z), halfSize);
				children[7] = OctTreeNode<T>(position + Vector3(halfSize.x, -halfSize.y, -halfSize.z), halfSize);
			}

			void DebugDraw() {

			}

			void OperateOnContents(OctTreeFunc& func) {
				if (children)
				{
					for (int i = 0; i < 8; ++i)
					{
						children[i].OperateOnContents(func);
					}
				}
				else 
				{
					if (!contents.empty())
					{
						func(contents);
					}
				}
			}

		protected:
			std::list< OctTreeEntry<T> >	contents;

			Vector3 position;
			Vector3 size;

			OctTreeNode<T>* children;
		};
	}
}


namespace NCL {
	using namespace NCL::Maths;
	namespace CSC8503 {
		template<class T>
		class OctTree
		{
		public:
			OctTree(Vector3 size, int maxDepth = 6, int maxSize = 5){
				root = OctTreeNode<T>(Vector3(), size);
				this->maxDepth	= maxDepth;
				this->maxSize	= maxSize;
			}
			~OctTree() {
			}

			void Insert(T object, const Vector3& pos, const Vector3& size) {
				root.Insert(object, pos, size, maxDepth, maxSize);
			}

			void DebugDraw() {
				root.DebugDraw();
			}

			void OperateOnContents(typename OctTreeNode<T>::OctTreeFunc  func) {
				root.OperateOnContents(func);
			}

		protected:
			OctTreeNode<T> root;
			int maxDepth;
			int maxSize;
		};
	}
}