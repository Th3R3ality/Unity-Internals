#include "hooks.hpp"

#include <iostream>

#include "UnityEngine/AssetBundle/AssetBundle.hpp"
#include "UnityEngine/Object/Object.hpp"
#include "UnityEngine/GameObject/GameObject.hpp"

#include "mscorlib/System/String/String.hpp"

#include "rust/classes/BasePlayer/BasePlayer.hpp"
#include "rust/classes/BaseNetworkable/BaseNetworkable.hpp"


#include "UnityEngine.Animations/UnityEngine.Animations.hpp"
#include "UnityEngine/Component/Component.hpp"

#include "cheat.hpp"
#include "cache.hpp"

UnityEngine::AssetBundle* bundle;
UnityEngine::Object* prefab;

void hk__BP_Load(BasePlayer* instance, BaseNetworkable::LoadInfo info)
{
	ORIG(hk__BP_Load);
	orig(instance, info);

	if (cache::check(instance))
		return;

	cache::add(instance);

	//UnityEngine::GameObject* go = instance->gameObject();

	std::cout << "bp_load hook!" << std::endl;

	auto go = instance->gameObject();
	if (!go) {
		std::cout << "fail - go" << std::endl;
		return;
	}

	auto name = go->name();
	if (!name) {
		std::cout << "fail - name" << std::endl;
		return;
	}
	std::wcout << name << std::endl;

	if (name->equals(L"LocalPlayer")) {
		cache::local(instance);
		std::cout << "set localplayer!" << std::endl;
	}

		
	auto bundle = cheat::load_assetbundle("C:\\Users\\reality\\Desktop\\monke.bundle");

	if (!prefab) {
		prefab = bundle->LoadAsset("assets/monke.prefab", UnityEngine::GameObject());
		std::cout << "loaded asset : " << prefab << std::endl;
	}

	UnityEngine::GameObject* monke = (UnityEngine::GameObject*)UnityEngine::Object::Instantiate(prefab);
	cache::go(instance, monke);
	std::cout << "monke : " << monke << "    added to    baseplayer : " << instance << std::endl;

    //if (monke) {
    if (false) {
		_transform(monke)->localPosition(_transform(instance)->localPosition());
		_transform(monke)->localRotation(_transform(instance)->localRotation());

        {
            using namespace UnityEngine::Animations;
            ParentConstraint* parentConstraint = (UnityEngine::Animations::ParentConstraint*)monke->AddComponent(UnityEngine::Animations::ParentConstraint());
            
            ConstraintSource* constraint = (ConstraintSource*)Il2cppLib::new_object("UnityEngine.Animations::ParentConstraint");
            constraint->sourceTransform(_transform(instance));
		    constraint->weight(1.f);
        
		    parentConstraint->AddSource(constraint);
        }
        


		


		((UnityEngine::Transform*)monke->transform())->position(((UnityEngine::Transform*)instance->transform())->position());

		/*

            private void AddRotationConstraint(GameObject objectToConstrain, GameObject targetObject){
                RotationConstraint rotationConstraint = objectToConstrain.AddComponent<RotationConstraint>();

                // Configure the Rotation Constraint to constrain to the target object
                rotationConstraint.constraintActive = true;
                rotationConstraint.AddSource(new ConstraintSource
                {
                    sourceTransform = targetObject.transform,
                    weight = 1f,
                });
                rotationConstraint.rotationOffset = new Vector3(0,0,0);
            }

            private Transform RecursiveFindChild(Transform parent, string name){
        
                for (int i = 0; i < parent.childCount; i++){
                    var child = parent.GetChild(i);
                    if (child.name == name){
                        return child;
                    }
                    var ret_ = RecursiveFindChild(child, name);
                    if (ret_ != null){
                        Debug.Log("Found Child : " + child.name);
                        return ret_;
                    }
                }
                return null;
            }

            //Loop All Bones
            //and
            //Apply Constraints

            private void LABaAC(Transform objectToConstrain, Transform target){
                for (int i = 0; i < objectToConstrain.childCount; i++){
                    var child = objectToConstrain.GetChild(i);
                    var res = RecursiveFindChild(target, child.name);
                    if (res != null){
                        AddRotationConstraint(child.gameObject, res.gameObject);
                    }
                    LABaAC(objectToConstrain.GetChild(i), target);
                }
            }
    
            public void AddParentConstraint(GameObject objectToConstrain, GameObject target)
            {
                objectToConstrain.transform.localPosition = target.transform.localPosition;
                objectToConstrain.transform.localRotation = target.transform.localRotation;

                ParentConstraint parentConstraint = objectToConstrain.AddComponent<ParentConstraint>();

                parentConstraint.AddSource(new ConstraintSource { sourceTransform = target.transform });

                parentConstraint.weight = 1f;
            }

            void Start()
            {
                GameObject target_base = GameObject.Find("monke1");

                GameObject source_base = Instantiate(monke_prefab, new Vector3(0,0,0), Quaternion.identity);

                Transform target = RecursiveFindChild(target_base.transform, "Pelvis");
                Transform source = RecursiveFindChild(source_base.transform, "Pelvis");

                var mesh_renderers = target_base.GetComponentsInChildren<MeshRenderer>();
                foreach (var mesh_renderer in mesh_renderers)
                {
                    mesh_renderer.gameObject.SetActive(false);
                }
                var skinned_mesh_renderers = target_base.GetComponentsInChildren<SkinnedMeshRenderer>();
                foreach (var skinned_mesh_renderer in skinned_mesh_renderers)
                {
                    skinned_mesh_renderer.gameObject.SetActive(false);
                }

                AddParentConstraint(source_base, target_base);

                LABaAC(source, target);
            }

		*/
	}
	
}