#include<iostream>
#include<graphics.h>//easyX
#include<vector>		//管理多架敌机
using namespace std;


//定义界面大小
constexpr auto swidth = 600;
constexpr auto sheight = 900;


bool PointInRec(int x, int y, RECT& r)			//创建一个布尔值来判断鼠标是否在矩形框内
{
	return (r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);		//判断的代码
}

//碰撞检测代码
bool RectCrashRect(RECT &r1, RECT &r2)
{
	RECT r;			//定义一个大矩形
	r.left = r1.left - (r2.right - r2.left);
	r.right = r1.right;
	r.top = r1.top - (r2.bottom - r2.top);
	r.bottom = r1.bottom;

	return (r.left < r2.left && r2.left <= r.right && r.top <= r2.top && r2.top <= r.bottom);			//判断是否碰撞的代码
}

//一个开始界面
void Welcome()
{
	//在画布特定位置画画
	//定义一个文字，是标题
	LPCTSTR title = _T("飞机大战");
	LPCTSTR tplay = _T("开始游戏");
	LPCTSTR texit = _T("退出游戏");

	RECT tplayr, texitr;				//这是一个结构体,tplayr是开始游戏的按钮
	BeginBatchDraw();					//减少标题闪烁

	setbkcolor(WHITE);					//设置背景为白色
	cleardevice();						//
	settextstyle(60, 0, _T("黑体"));		//设置字体规格
	settextcolor(BLACK);				//设置字体颜色为黑色
	outtextxy(swidth / 2 - textwidth(title) / 2, sheight / 10, title);		//显示标题的位置

	settextstyle(40, 0, _T("黑体"));										//设置字体的大小,需要在获得函数之前的位置定义出来,否则会歪
	tplayr.left = swidth / 2 - textwidth(tplay) / 2;					//确定开始游戏图标的坐标
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = sheight / 5 * 2.5;
	tplayr.bottom = tplayr.top + textheight(tplay);

	texitr.left = swidth / 2 - textwidth(texit) / 2;					//确定退出游戏图标的坐标
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = sheight / 5 * 3;
	texitr.bottom = texitr.top + textheight(texit);


	outtextxy(tplayr.left, tplayr.top, tplay);						//显示开始图标
	outtextxy(texitr.left, texitr.top, texit);						//显示退出图标


	EndBatchDraw();						//减少标题闪烁
	/*Sleep(100);*/

	//创建鼠标点击事件
	while (true)		//循环获取鼠标点击事件
	{
		/*定义一个结构体来获取得到的信息*/
		ExMessage mess;							//储存获取的信息
		getmessage(&mess, EX_MOUSE);			//获取鼠标的行为信息
		if (mess.lbutton)						//当左键按下时，lbutton = 1
		{						//如果鼠标在矩形的范围内点击，则说明用户点击了图标
			if (PointInRec(mess.x, mess.y, tplayr))
			{
				return;			//return完之后就是主循环
			}
			else if (PointInRec(mess.x, mess.y, texitr))			//若是点击退出按钮
			{
				exit(0);		//系统级，直接退出程序
			}
		}
	}

}

//背景，敌机，英雄，子弹
class BK	//背景类
{
public:
	BK(IMAGE& img)		//构造函数
		:img(img), y(-sheight)				//不需要x，只需要屏幕高度的相反数
	{					//引用需要第一时间初始化

	}
	void Show()
	{
		if (y == 0) { y = -sheight; }		//如果图片滚到底部，则从新设置高度
		y += 4;						//实现向下滚动效果，*加的的数必须得是高度的倍数，才能使y值跑到0
		putimage(0, y, &img);		//图片
	}

private:
	IMAGE& img;		//引用

	int y;		//定义一个x坐标和y坐标
};

class Hero
{
public:
	Hero(IMAGE& img)
		:img(img)
	{
		rect.left = swidth / 2 - img.getwidth() / 2;			//中间
		rect.top = sheight - img.getheight();					//高度
		rect.right = rect.left + img.getwidth();
		rect.bottom = sheight;
	}

	void Show()
	{
		//生成飞机的图片
		putimage(rect.left, rect.top, &img);
	}

	void Control()		
	{
		//使用鼠标控制飞机
		ExMessage mess;
		if (peekmessage(&mess, EX_MOUSE))		//判断是否获得消息,获取的鼠标消息
		{
			rect.left = mess.x - img.getwidth() / 2;		//使鼠标在图像的中心
			rect.top = mess.y - img.getheight() / 2;
			rect.right = rect.right = rect.left + img.getwidth();			//更新图片位置
			rect.bottom = rect.top + img.getheight();

		}

	}

	//获取玩家飞机的边框
	RECT& GetRect() { return rect; }

private:
	IMAGE& img;
	RECT rect;				//矩形边框，判断是否与敌军相撞


};

class Enemy
{
public:
	Enemy(IMAGE& img, int x)	//需手动输入一个x坐标			//在构造函数中传入自己的img
		:img(img)
	{
		rect.left = x;		
		rect.right = rect.left + img.getwidth();
		rect.top = -img.getheight();			//生成在屏幕外,再缓慢入场
		rect.bottom = 0;
	}

	bool Show()			//敌机的展示		,使用布尔值,若飞出画面和被子弹击中时返回一个false
	{
		//飞出画面
		if (rect.top >= sheight)			//若图片顶点大于屏幕高度,则一定飞出屏幕		
		{
			return false;
		}
		rect.top += 6;			//若没有飞出,则不断向下移动
		rect.bottom += 6;			//更改敌机飞行速度
		putimage(rect.left, rect.top, &img);			//生成图片

		return true;
	}
	RECT& GetRect() { return rect; }

private:
	IMAGE& img;						//有自己的img类
	RECT rect;				//创建矩形框架
};

class Bullet
{
public:
	Bullet(IMAGE& img,RECT pr)			//子弹是在我方飞机前生成的，所以也需要我方飞机的位置
		:img(img)
	{
		rect.left = pr.left + (pr.right - pr.left) / 2 - img.getwidth() / 2;			//求出子弹的位置
		rect.right = rect.left + img.getwidth();
		rect.top = pr.top - img.getheight();
		rect.bottom = rect.top + img.getheight();
	}

	bool Show()
	{
		if (rect.bottom <= 0)			//底边小于等于0，则说明子弹已经飞出
		{
			return false;			//不对该子弹进行刷新
		}
		rect.top -= 10;				//刷新位置
		rect.bottom -= 10;			//子弹飞行速度
		putimage(rect.left, rect.top, &img);
	}

	RECT& GetRect() { return rect; }			//返回矩形

private:
	IMAGE& img;			//img引用
	RECT rect;
};

bool AddEnemy(vector<Enemy*> &es,IMAGE& enemyimg)			//以敌机列表作为参数
{
	Enemy* e = new Enemy(enemyimg, abs(rand()) % (swidth - enemyimg.getwidth()));
	for (auto& i : es)			//for循环遍历生成好的敌机
	{
		if (RectCrashRect(i->GetRect(), e->GetRect()))			//当前遍历的rect和敌机生成的rect出现了碰撞
		{
			delete e;				//产生碰撞则删除生成的敌机
			return false;
		}
	}
	es.push_back(e);			//若没有问题，则可以生成
	return true;
}

//游戏界面代码
bool Play()
{
	setbkcolor(WHITE);							//设置背景为白色
	cleardevice();								//进行一次刷新
	bool is_play = true;
	IMAGE heroimg, enemyimg, bkimg, bulletimg;				//图形

	loadimage(&heroimg, _T("H:\\CodeResource\\PlaneFight_study\\images\\me1.png"));			//图片的路径
	loadimage(&enemyimg, _T("H:\\CodeResource\\PlaneFight_study\\images\\enemy1.png"));
	loadimage(&bkimg, _T("H:\\CodeResource\\PlaneFight_study\\images\\bk3.png"), swidth, sheight * 2);		//进行一次格式化
	loadimage(&bulletimg, _T("H:\\CodeResource\\PlaneFight_study\\images\\bullet1.png"));

	BK bk = BK(bkimg);				//实例化的背景对象
	Hero hp = Hero(heroimg);			//实例化一个英雄对象，传入英雄图片

	vector<Enemy*> es;			//敌机类指针容器
	vector<Bullet*> bs;				//子弹类指针容器
	int bsing = 0;
 	for (int i = 0; i < 5; i++)			//更改敌机数量
	{
		AddEnemy(es, enemyimg);			//初始化一个敌机,参数为敌机的照片,随机生成一个x坐标,但注意右边需要留出和敌机宽度一样的宽度
	}

	//图形界面

	while (is_play)								//游戏的主循环
	{
		bsing++;
		if (bsing == 15)			//子弹开火频率
		{
			bsing = 0;
			bs.push_back(new Bullet(bulletimg, hp.GetRect()));			//生成子弹，子弹的图片和位置
		}

		BeginBatchDraw();						//防止屏闪

		bk.Show();								//调用的函数
		Sleep(6);								//
		flushmessage();							//刷新消息的缓冲区，以获得最新的消息
		Sleep(2);								//sleep一下再获得消息
		hp.Control();
										
		hp.Show();								//生成玩家的飞机

		//Sleep(16);								//到达每秒60帧的速度

		for (auto& i : bs)			//遍历生成子弹
		{
			i->Show();
		}

		auto it = es.begin();			//定义一个指针让他等于容器的初始位置	
		while (it != es.end())			//若没到头,则继续进行循环
		{
			if (RectCrashRect((*it)->GetRect(), hp.GetRect()))			//当前检测的敌机和玩家飞机是否相撞,若返回true则代表相撞了
			{
				is_play = false;			//游戏失败
			}
			auto bit = bs.begin();			//遍历子弹销毁
			while (bit != bs.end())
			{
				if (RectCrashRect((*bit)->GetRect(), (*it)->GetRect()))			//遍历检测每个子弹和敌机的位置碰撞
				{
					delete(*it);			//若返回为true，则说明发生碰撞，删除敌机
					es.erase(it);
					it = es.begin();
					delete(*bit);			//删除子弹
					bs.erase(bit);
					break;
				}
				else bit++;
			}
			if (!(*it)->Show())			//它若返回false,则说明飞出去了,要对其进行销毁
			{
				delete(*it);			//删除new出来的指针
				es.erase(it);		//视频中另一个方法解决报错
				it = es.begin();			//使用销毁函数,返回it指针的下一个值,否则没法循环了
			}
							//else用于解决一个报错
				it++;
						//如果没被销毁,则对指针进行++,指向下一个
		}

		for (int i = 0; i < 5 - es.size(); i++)				//对敌机动态数量增加
		{
			AddEnemy(es, enemyimg);			//添加敌机
		}
		EndBatchDraw();
	}
	return true;
}

int main() {
	//easyX初始化
	initgraph(swidth, sheight, EX_NOMINIMIZE | EX_SHOWCONSOLE);
	bool is_live = true;		//游戏是否在进行
	while (is_live)				//如果游戏在进行，则持续进行
	{
		Welcome();				//打开开始界面
		//游戏界面函数
		is_live = Play();		//游戏主循环是否为true，若返回的play()结果为false,则is_live为false,则退出主循环来结束游戏进程
	}

	return 0;
}