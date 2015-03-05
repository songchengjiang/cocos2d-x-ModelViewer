#include "HelloWorldScene.h"

USING_NS_CC;
//include for particle
#include "Particle3D/CCParticle3DEmitter.h"
#include "Particle3D/CCParticle3DAffector.h"
#include "Particle3D/CCParticle3DRender.h"
#include "Particle3D/ParticleUniverse/ParticleRenders/CCPUParticle3DRender.h"
#include "Particle3D/ParticleUniverse/ParticleEmitters/CCPUParticle3DPointEmitter.h"
#include "Particle3D/ParticleUniverse/ParticleEmitters/CCPUParticle3DBoxEmitter.h"
#include "Particle3D/ParticleUniverse/ParticleEmitters/CCPUParticle3DCircleEmitter.h"
#include "Particle3D/ParticleUniverse/ParticleEmitters/CCPUParticle3DLineEmitter.h"
#include "Particle3D/ParticleUniverse/ParticleEmitters/CCPUParticle3DPositionEmitter.h"
#include "Particle3D/ParticleUniverse/ParticleEmitters/CCPUParticle3DSphereSurfaceEmitter.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DAlignAffector.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DBoxCollider.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DColorAffector.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DGravityAffector.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DScaleAffector.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DFlockCenteringAffector.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DForceFieldAffector.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DLinearForceAffector.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DParticleFollower.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DPathFollower.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DPlaneCollider.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DRandomiser.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DSineForceAffector.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DTextureRotator.h"
#include "Particle3D/ParticleUniverse/ParticleAffectors/CCPUParticle3DVortexAffector.h"
#include "Particle3D/ParticleUniverse/CCPUParticleSystem3D.h"
#include "Particle3D/ParticleUniverse/CCPUParticle3DScriptCompiler.h"



Scene* ModelViewer::createScene(const std::string &filePath)
{

    ////add search path for the Particle
    //FileUtils::getInstance()->addSearchPath("materials");
    //FileUtils::getInstance()->addSearchPath("scripts");

    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = ModelViewer::create();
    layer->loadModelOrParticle(filePath);
    layer->setCamera();
    layer->resetCamera();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ModelViewer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto listenertouch = EventListenerTouchAllAtOnce::create();
    listenertouch->onTouchesMoved = CC_CALLBACK_2(ModelViewer::onTouchsMovedThis, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenertouch, this);

    auto listenermouse= EventListenerMouse::create();
    listenermouse->onMouseScroll = CC_CALLBACK_1(ModelViewer::onMouseScrollThis, this);
    listenermouse->onMouseMove = CC_CALLBACK_1(ModelViewer::onMouseMovedThis, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenermouse, this);

    auto listenerkeyboard= EventListenerKeyboard::create();
    listenerkeyboard->onKeyPressed = CC_CALLBACK_2(ModelViewer::onKeyPressedThis, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyboard, this);

    _layer = Layer::create();
    _layer->retain();
    addChild(_layer);

    return true;
}

void ModelViewer::loadModelOrParticle( const std::string &filePath )
{
    char extension [32];
    getFileExtension(filePath.c_str(),extension);
    if(strcmp(extension,"c3b")==0 ||strcmp(extension,"c3t")==0 )
    {
        loadModel(filePath);
    }else if(strcmp(extension,"pu")==0 )
    {
        loadParticle(filePath);
    }
}

ModelViewer::ModelViewer()
    : _camera(nullptr)
    , _layer(nullptr)
    , _orginDistance(0.0f)
    , _distance(0.0f)
    , _trackballSize(1.0f)
{

}

ModelViewer::~ModelViewer()
{
    if (_layer)
        _layer->release();

    if (_camera)
        _camera->release();
}

void ModelViewer::onTouchsMovedThis( const std::vector<Touch*> &touchs, Event *event )
{
    if (!touchs.empty())
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 prelocation = touchs[0]->getPreviousLocationInView();
        Vec2 location = touchs[0]->getLocationInView();
        location.x = 2.0 * (location.x) / (visibleSize.width) - 1.0f;
        location.y = 2.0 * (visibleSize.height - location.y) / (visibleSize.height) - 1.0f;
        prelocation.x = 2.0 * (prelocation.x) / (visibleSize.width) - 1.0f;
        prelocation.y = 2.0 * (visibleSize.height - prelocation.y) / (visibleSize.height) - 1.0f;

        Vec3 axes;
        float angle;
        trackball(axes, angle, prelocation.x, prelocation.y, location.x, location.y);
        Quaternion quat(axes, angle);
        _rotation = quat * _rotation;

        updateCameraTransform();
    }
}

void ModelViewer::onMouseScrollThis( Event* event )
{
    EventMouse *em = dynamic_cast<EventMouse *>(event);
    if (em)
    {
        _distance += em->getScrollY() * _orginDistance * 0.01f;

        updateCameraTransform();
    }
}

void ModelViewer::onMouseMovedThis( Event* event )
{
    EventMouse *em = dynamic_cast<EventMouse *>(event);
    if (em)
    {
        if (em->getMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
        {
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 delta = (em->getLocation() - _preMouseLocation);
            Mat4 rot;
            Mat4::createRotation(_rotation, &rot);
            _center += rot * -Vec3(delta.x / visibleSize.width, (-delta.y) / visibleSize.height, 0.0f) * _orginDistance * 0.1f;
            updateCameraTransform();
        }

        _preMouseLocation = em->getLocation();
    }
}

float ModelViewer::tb_project_to_sphere( float r, float x, float y )
{
    float d, t, z;
    d = sqrt(x*x + y*y);
    if (d < r * 0.70710678118654752440)
    {
        z = sqrt(r*r - d*d);
    }                         
    else
    {
        t = r / 1.41421356237309504880;
        z = t*t / d;
    }
    return z;
}

void ModelViewer::trackball( cocos2d::Vec3 & axis, float & angle, float p1x, float p1y, float p2x, float p2y )
{
    Mat4 rotation_matrix;
    Mat4::createRotation(_rotation, &rotation_matrix);

    Vec3 uv = rotation_matrix * Vec3(0.0f,1.0f,0.0f);
    Vec3 sv = rotation_matrix * Vec3(1.0f,0.0f,0.0f);
    Vec3 lv = rotation_matrix * Vec3(0.0f,0.0f,-1.0f);

    Vec3 p1 = sv * p1x + uv * p1y - lv * tb_project_to_sphere(_trackballSize, p1x, p1y);
    Vec3 p2 = sv * p2x + uv * p2y - lv * tb_project_to_sphere(_trackballSize, p2x, p2y);

    Vec3::cross(p2, p1, &axis);
    axis.normalize();

    float t = (p2 - p1).length() / (2.0 * _trackballSize);

    if (t > 1.0) t = 1.0;
    if (t < -1.0) t = -1.0;
    angle = asin(t);
}

void ModelViewer::updateCameraTransform()
{
    Mat4 trans, rot, center;
    Mat4::createTranslation(Vec3(0.0f, 0.0f, _distance), &trans);
    Mat4::createRotation(_rotation, &rot);
    Mat4::createTranslation(_center, &center);
    Mat4 result = center * rot * trans;
    _camera->setNodeToParentTransform(result);
}

void ModelViewer::onKeyPressedThis( EventKeyboard::KeyCode keycode, Event *event )
{
    switch (keycode)
    {
    case EventKeyboard::KeyCode::KEY_SPACE:
        {
            resetCamera();
        }
        break;
    default:
        break;
    }
}

void ModelViewer::resetCamera()
{
    _distance = _orginDistance;
    _center = _orginCenter;
    _rotation.set(0.0f, 0.0f, 0.0f, 1.0f);
    _camera->setPosition3D(_orginCenter + Vec3(0.0f, 0.0f,  _orginDistance));
    _camera->lookAt(_orginCenter, Vec3(0.0f, 1.0f, 0.0f));
}

void ModelViewer::setCamera()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _camera = Camera::createPerspective(60.0f, (GLfloat)visibleSize.width/visibleSize.height, 1.0f, _orginDistance * 5.0f);
    _camera->setCameraFlag(CameraFlag::USER1);
    _camera->setPosition3D(Vec3(0.0f, 0.0f, 10.0f));
    _camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    _camera->retain();
    _camera->setCameraMask((unsigned short)CameraFlag::USER1);
    _layer->addChild(_camera);
}

void ModelViewer::loadModel(const std::string &filePath)
{
    Sprite3D *sprite = Sprite3D::create(filePath);
    if (sprite)
    {
        auto animation = Animation3D::create(filePath);
        if (animation)
        {
            auto animate = Animate3D::create(animation);
            sprite->runAction(RepeatForever::create(animate));
        }

        AABB aabb = sprite->getAABB();
        Vec3 corners[8];
        aabb.getCorners(corners);
        //temporary method, replace it
        if (abs(corners[3].x) == 99999.0f && abs(corners[3].y) == 99999.0f && abs(corners[3].z) == 99999.0f)
        {
            _orginCenter = Vec3(0.0f, 0.0f, 0.0f);
            _orginDistance = 100.0f;
        }
        else
        {
            float radius = (corners[0] - corners[5]).length();
            _orginCenter = aabb.getCenter();
            _orginDistance = radius;
        }
        sprite->setCameraMask((unsigned short)CameraFlag::USER1);
        _layer->addChild(sprite);
    }

}

void ModelViewer::loadParticle(const std::string &filePath)
{
    auto rootps = PUParticleSystem3D::create(filePath);
    rootps->setCameraMask((unsigned short)CameraFlag::USER1);
    rootps->startParticleSystem();
    _orginCenter = Vec3(0.0f, 0.0f, 0.0f);
    _orginDistance = 50.0f;
    _layer->addChild(rootps);
}

void ModelViewer::getFileExtension(const char *file_name,char *extension)
{
    int i=0,length;  
    length=strlen(file_name);  
    while(file_name[i])  
    {  
        if(file_name[i]=='.')  
            break;  
        i++;  
    }  
    if(i<length)  
        strcpy(extension,file_name+i+1);  
    else  
        strcpy(extension,"\0");  
}
