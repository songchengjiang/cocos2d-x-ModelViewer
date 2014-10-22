#ifndef __MODELVIEWER_SCENE_H__
#define __MODELVIEWER_SCENE_H__

#include "cocos2d.h"

class ModelViewer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(const std::string &filePath);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    void loadModel(const std::string &filePath);

    // implement the "static create()" method manually
    CREATE_FUNC(ModelViewer);

CC_CONSTRUCTOR_ACCESS:
    ModelViewer();
    virtual ~ModelViewer();

protected:

    void onTouchsMovedThis(const std::vector<cocos2d::Touch*> &touchs, cocos2d::Event *event);
    void onMouseScrollThis(cocos2d::Event* event);
    void onMouseMovedThis(cocos2d::Event* event);
    void onKeyPressedThis(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);

    void trackball( cocos2d::Vec3 & axis, float & angle, float p1x, float p1y, float p2x, float p2y );
    float tb_project_to_sphere( float r, float x, float y );

    void updateCameraTransform();
    void resetCamera();


private:

    cocos2d::Camera *_camera;
    cocos2d::Layer *_layer;

    float _orginDistance;
    cocos2d::Vec3 _orginCenter;

    float _distance;
    float _trackballSize;
    cocos2d::Quaternion _rotation;
    cocos2d::Vec3 _center;
    cocos2d::Vec2 _preMouseLocation;
};

#endif // __HELLOWORLD_SCENE_H__
