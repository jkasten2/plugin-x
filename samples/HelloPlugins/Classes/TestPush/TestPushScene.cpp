/****************************************************************************
Copyright (c) 2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "TestPushScene.h"
#include "MyPushManager.h"
#include "HelloWorldScene.h"

#define COCOS2D_DEBUG 1

USING_NS_CC;

Label* TestPush::newMessageLabel;


Scene* TestPush::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    TestPush *layer = TestPush::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TestPush::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
        return false;

    CCLog("TestPush::init(");

    MyPushManager::getInstance()->loadPlugin();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    Point posMid = Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    Point posBR = Point(origin.x + visibleSize.width, origin.y);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    MenuItemFont *pBackItem = MenuItemFont::create("Back", CC_CALLBACK_1(TestPush::menuBackCallback, this));
    Size backSize = pBackItem->getContentSize();
    pBackItem->setPosition(posBR + Point(- backSize.width / 2, backSize.height / 2));

    // create menu, it's an autorelease object
    Menu* pMenu = Menu::create(pBackItem, NULL);
    pMenu->setPosition(Point::ZERO);

    Label* label1 = Label::create("Init", "Arial", 32);
    MenuItemLabel* pItemLogin = MenuItemLabel::create(label1, CC_CALLBACK_1(TestPush::testInit, this));
    pItemLogin->setAnchorPoint(Point(0.5f, 0));
    pMenu->addChild(pItemLogin, 0);
    pItemLogin->setPosition(posMid + Point(-100, -120));

    Label* label2 = Label::create("sendTag", "Arial", 32);
    MenuItemLabel* pItemLogout = MenuItemLabel::create(label2, CC_CALLBACK_1(TestPush::testSendTag, this));
    pItemLogout->setAnchorPoint(Point(0.5f, 0));
    pMenu->addChild(pItemLogout, 0);
    pItemLogout->setPosition(posMid + Point(100, -120));

    // create optional menu
    // cases item
    _caseItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(TestPush::caseChanged, this),
                                                MenuItemFont::create( "GameThrive" ),
                                                NULL );
    int caseLen = 2 / sizeof(std::string);
    _caseItem->getSubItems().pushBack( MenuItemFont::create( "GameThrive" ) );
    _caseItem->setPosition(posMid + Point(0, 120));
    pMenu->addChild(_caseItem);

    this->addChild(pMenu, 1);
    
    
    TestPush::newMessageLabel = Label::create("", "Arial", 25);
    TestPush::newMessageLabel->setPosition(posMid + Point(0, -200));
    this->addChild(TestPush::newMessageLabel, 0);
    
    return true;
}

void TestPush::onPushReceived(const char* message) {
    TestPush::newMessageLabel->setString(message);
}

void TestPush::caseChanged(Ref* pSender) {
}

void TestPush::testInit(Ref* pSender) {
    // NOTE: Init is called from a button for testing here but you should always call init when app first starts so notifications are not missed.
    MyPushManager::getInstance()->init((PushReceived)onPushReceived);
}

void TestPush::testSendTag(Ref* pSender) {
    MyPushManager::getInstance()->sendTag();
}

void TestPush::menuBackCallback(Ref* pSender)
{
    MyPushManager::purgeManager();

    Scene* newScene = HelloWorld::scene();
    Director::getInstance()->replaceScene(newScene);
}
