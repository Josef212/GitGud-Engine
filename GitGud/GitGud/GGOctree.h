#ifndef __GGOCTREE_H__
#define __GGOCTREE_H__

#include "Math.h"

#include <vector>
#include <stack>
#include <queue>
#include <map>
#include "GameObject.h"

#define NODE_MAX_ITEMS 10

class GGOctreeNode
{
	friend class GGOctree;
public:
	GGOctreeNode()
	{}
	GGOctreeNode(GGOctreeNode* parent, const AABB& box) : parent(parent), box(box)
	{
		for (unsigned int i = 0; i < 8; ++i)
			childs[i] = nullptr;
	}

	~GGOctreeNode()
	{
		for (unsigned int i = 0; i < 8; ++i)
			if (childs[i]) delete childs[i];
	}

	void Insert(GameObject* obj)
	{
		if (!obj)
			return;

		if (childs[0] == nullptr && objects.size() < NODE_MAX_ITEMS)
		{
			objects.push_back(obj);
		}
		else
		{
			if (childs[0] == nullptr)
				DivideNode();

			objects.push_back(obj);
			AjustNode();
		}
	}

	void Erase(GameObject* obj)
	{
		std::vector<GameObject*>::iterator it = std::find(objects.begin(), objects.end(), obj);
		if (it != objects.end())
			objects.erase(it);

		if (childs[0])
		{
			for (auto tmp : childs)
			{
				tmp->Erase(obj);
			}
		}
	}

	void CollectCandidates(std::vector<GameObject*>& vec, const Frustum& collector)
	{
		if (collector.Intersects(box))
		{
			for (std::vector<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
			{
				vec.push_back((*it));
			}
		}

		if (childs[0])
		{
			for (auto tmp : childs)
			{
				tmp->CollectCandidates(vec, collector);
			}
		}

	}

	void CollectBoxes(std::vector<AABB>& vec)
	{
		vec.push_back(box);

		if (childs[0])
		{
			for (auto tmp : childs)
			{
				tmp->CollectBoxes(vec);
			}
		}
	}

	template<typename TYPE>
	void CollectIntersections(std::map<float, GameObject*>& objects, const TYPE& primitive)const;
	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive)const;

private:
	void DivideNode()
	{
		float3 center = box.CenterPoint();
		float3 center2 = float3::zero;
		float3 size = box.Size();
		float3 size2(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
		AABB tmp;

		float sx = size.x * 0.25f;
		float sy = size.y * 0.25f;
		float sz = size.z * 0.25f;

		//----------TOP----------
		//----------North-east
		center2.Set(center.x + sx, center.y + sy, center.z + sz);
		tmp.SetFromCenterAndSize(center2, size2);
		childs[0] = new GGOctreeNode(this, tmp);

		//----------South-east
		center2.Set(center.x + sx, center.y + sy, center.z - sz);
		tmp.SetFromCenterAndSize(center2, size2);
		childs[1] = new GGOctreeNode(this, tmp);

		//----------South-west
		center2.Set(center.x - sx, center.y + sy, center.z - sz);
		tmp.SetFromCenterAndSize(center2, size2);
		childs[2] = new GGOctreeNode(this, tmp);

		//----------North-east
		center2.Set(center.x - sx, center.y + sy, center.z + sz);
		tmp.SetFromCenterAndSize(center2, size2);
		childs[3] = new GGOctreeNode(this, tmp);

		//----------BOT----------
		//----------North-east
		center2.Set(center.x + sx, center.y - sy, center.z + sz);
		tmp.SetFromCenterAndSize(center2, size2);
		childs[4] = new GGOctreeNode(this, tmp);

		//----------South-east
		center2.Set(center.x + sx, center.y - sy, center.z - sz);
		tmp.SetFromCenterAndSize(center2, size2);
		childs[5] = new GGOctreeNode(this, tmp);

		//----------South-west
		center2.Set(center.x - sx, center.y - sy, center.z - sz);
		tmp.SetFromCenterAndSize(center2, size2);
		childs[6] = new GGOctreeNode(this, tmp);

		//----------North-east
		center2.Set(center.x - sx, center.y - sy, center.z + sz);
		tmp.SetFromCenterAndSize(center2, size2);
		childs[7] = new GGOctreeNode(this, tmp);

	}

	void AjustNode()
	{
		auto it = objects.begin();
		while (it != objects.end())
		{
			GameObject* tmp = *it;
			AABB b(tmp->enclosingBox);

			bool intersections[8];
			for (unsigned int i = 0; i < 8; ++i)
			{
				intersections[i] = childs[i]->box.Intersects(b);
			}

			if (intersections[0] && intersections[1] && intersections[2] && intersections[3] &&
				intersections[4] && intersections[5] && intersections[6] && intersections[7])
			{
				++it;
			}
			else
			{
				it = objects.erase(it);
				for (unsigned int i = 0; i < 8; ++i)
				{
					if (intersections[i])
					{
						childs[i]->Insert(tmp);
					}
				}
			}
		}
	}

private:
	AABB box;
	std::vector<GameObject*> objects;
	GGOctreeNode* parent = nullptr;
	GGOctreeNode* childs[8];
};


class GGOctree
{
public:
	GGOctree()
	{}

	virtual ~GGOctree()
	{
		Clear();
	}

	void Create(const AABB box)
	{
		if (root)
			Clear();
		root = new GGOctreeNode(nullptr, box);
	}

	void Insert(GameObject* obj)
	{
		if (root && obj)
			if(obj->enclosingBox.Intersects(root->box))
				root->Insert(obj);
	}

	void Erase(GameObject* obj)
	{
		if (root && obj)
			root->Erase(obj);
	}

	void CollectCandidates(std::vector<GameObject*>& vec, const Frustum& collector)
	{
		if (root)
			if(collector.Intersects(root->box))
				root->CollectCandidates(vec, collector);
	}

	void CollectBoxes(std::vector<AABB>& vec)
	{
		if (root)
			root->CollectBoxes(vec);
	}

	template<typename TYPE>
	void CollectIntersections(std::map<float, GameObject*>& objects, const TYPE& primitive)const;
	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive)const;

private:
	void Clear()
	{
		if (root)
			delete root;
		root = nullptr;
	}

public:
	GGOctreeNode* root = nullptr;
};

//-------------------------------------------------------

template<typename TYPE>
inline void GGOctree::CollectIntersections(std::map<float, GameObject*>& objects, const TYPE& primitive)const
{
	if (root)
		root->CollectIntersections(objects, primitive);
}

template<typename TYPE>
inline void GGOctree::CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive)const
{
	if (root)
		root->CollectIntersections(objects, primitive);
}



template<typename TYPE>
inline void GGOctreeNode::CollectIntersections(std::map<float, GameObject*>& objects, const TYPE& primitive)const
{
	if (primitive.Intersects(box))
	{
		float nearHit, farHit;
		for (std::vector<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		{
			if (primitive.Intersects((*it)->enclosingBox, nearHit, farHit))
				objects[nearHit] = *it;
		}

		for (unsigned int i = 0; i < 8; ++i)
			if (childs[i]) childs[i]->CollectIntersections(objects, primitive);
	}
}

template<typename TYPE>
inline void GGOctreeNode::CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive)const
{
	if (primitive.Intersects(box))
	{
		for (std::vector<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		{
			if (primitive.Intersects((*it)->enclosingBox))
				objects.push_back(*it);
		}

		for (unsigned int i = 0; i < 8; ++i)
			if (childs[i]) childs[i]->CollectIntersections(objects, primitive);
	}
}

#endif // !__GGOCTREE_H__