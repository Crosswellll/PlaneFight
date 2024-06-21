#include<iostream>
#include<graphics.h>//easyX
#include<vector>		//�����ܵл�
using namespace std;


//��������С
constexpr auto swidth = 600;
constexpr auto sheight = 900;


bool PointInRec(int x, int y, RECT& r)			//����һ������ֵ���ж�����Ƿ��ھ��ο���
{
	return (r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);		//�жϵĴ���
}

//��ֹ�л�����ʱ�ص�
bool RectCrashRect(RECT &r1, RECT &r2)
{
	RECT r;			//����һ�������
}

//һ����ʼ����
void Welcome()
{
	//�ڻ����ض�λ�û���
	//����һ�����֣��Ǳ���
	LPCTSTR title = _T("�ɻ���ս");
	LPCTSTR tplay = _T("��ʼ��Ϸ");
	LPCTSTR texit = _T("�˳���Ϸ");

	RECT tplayr, texitr;				//����һ���ṹ��,tplayr�ǿ�ʼ��Ϸ�İ�ť
	BeginBatchDraw();					//���ٱ�����˸

	setbkcolor(WHITE);					//���ñ���Ϊ��ɫ
	cleardevice();						//
	settextstyle(60, 0, _T("����"));		//����������
	settextcolor(BLACK);				//����������ɫΪ��ɫ
	outtextxy(swidth / 2 - textwidth(title) / 2, sheight / 10, title);		//��ʾ�����λ��

	settextstyle(40, 0, _T("����"));										//��������Ĵ�С,��Ҫ�ڻ�ú���֮ǰ��λ�ö������,�������
	tplayr.left = swidth / 2 - textwidth(tplay) / 2;					//ȷ����ʼ��Ϸͼ�������
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = sheight / 5 * 2.5;
	tplayr.bottom = tplayr.top + textheight(tplay);

	texitr.left = swidth / 2 - textwidth(texit) / 2;					//ȷ���˳���Ϸͼ�������
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = sheight / 5 * 3;
	texitr.bottom = texitr.top + textheight(texit);


	outtextxy(tplayr.left, tplayr.top, tplay);						//��ʾ��ʼͼ��
	outtextxy(texitr.left, texitr.top, texit);						//��ʾ�˳�ͼ��


	EndBatchDraw();						//���ٱ�����˸
	/*Sleep(100);*/

	//����������¼�
	while (true)		//ѭ����ȡ������¼�
	{
		/*����һ���ṹ������ȡ�õ�����Ϣ*/
		ExMessage mess;							//�����ȡ����Ϣ
		getmessage(&mess, EX_MOUSE);			//��ȡ������Ϊ��Ϣ
		if (mess.lbutton)						//���������ʱ��lbutton = 1
		{						//�������ھ��εķ�Χ�ڵ������˵���û������ͼ��
			if (PointInRec(mess.x, mess.y, tplayr))
			{
				return;			//return��֮�������ѭ��
			}
			else if (PointInRec(mess.x, mess.y, texitr))			//���ǵ���˳���ť
			{
				exit(0);		//ϵͳ����ֱ���˳�����
			}
		}
	}

}

//�������л���Ӣ�ۣ��ӵ�

class BK	//������
{
public:
	BK(IMAGE& img)		//���캯��
		:img(img), y(-sheight)				//����Ҫx��ֻ��Ҫ��Ļ�߶ȵ��෴��
	{					//������Ҫ��һʱ���ʼ��

	}
	void Show()
	{
		if (y == 0) { y = -sheight; }		//���ͼƬ�����ײ�����������ø߶�
		y += 4;						//ʵ�����¹���Ч����*�ӵĵ���������Ǹ߶ȵı���������ʹyֵ�ܵ�0
		putimage(0, y, &img);		//ͼƬ
	}

private:
	IMAGE& img;		//����

	int y;		//����һ��x�����y����
};

class Hero
{
public:
	Hero(IMAGE& img)
		:img(img)
	{
		rect.left = swidth / 2 - img.getwidth() / 2;			//�м�
		rect.top = sheight - img.getheight();					//�߶�
		rect.right = rect.left + img.getwidth();
		rect.bottom = sheight;
	}

	void Show()
	{
		//���ɷɻ���ͼƬ
		putimage(rect.left, rect.top, &img);
	}

	void Control()		
	{
		//ʹ�������Ʒɻ�
		ExMessage mess;
		if (peekmessage(&mess, EX_MOUSE))		//�ж��Ƿ�����Ϣ,��ȡ�������Ϣ
		{
			rect.left = mess.x - img.getwidth() / 2;		//ʹ�����ͼ�������
			rect.top = mess.y - img.getheight() / 2;
			rect.right = rect.right = rect.left + img.getwidth();			//����ͼƬλ��
			rect.bottom = rect.top + img.getheight();

		}

	}

private:
	IMAGE& img;
	RECT rect;				//���α߿��ж��Ƿ���о���ײ


};

class Enemy
{
public:
	Enemy(IMAGE& img, int x)	//���ֶ�����һ��x����			//�ڹ��캯���д����Լ���img
		:img(img)
	{
		rect.left = x;		
		rect.right = rect.left + img.getwidth();
		rect.top = -img.getheight();			//��������Ļ��,�ٻ����볡
		rect.bottom = 0;
	}

	bool Show()			//�л���չʾ		,ʹ�ò���ֵ,���ɳ�����ͱ��ӵ�����ʱ����һ��false
	{
		//�ɳ�����
		if (rect.top >= sheight)			//��ͼƬ���������Ļ�߶�,��һ���ɳ���Ļ		
		{
			return false;
		}
		rect.top += 2;			//��û�зɳ�,�򲻶������ƶ�
		rect.bottom += 2;
		putimage(rect.left, rect.top, &img);			//����ͼƬ

		return true;
	}


private:
	IMAGE& img;						//���Լ���img��
	RECT rect;				//�������ο��
};

//��Ϸ�������
bool Play()
{
	setbkcolor(WHITE);							//���ñ���Ϊ��ɫ
	cleardevice();								//����һ��ˢ��
	bool is_play = true;
	IMAGE heroimg, enemyimg, bkimg, bulletimg;				//ͼ��

	loadimage(&heroimg, _T("H:\\CodeResource\\PlaneFight_study\\images\\me1.png"));			//ͼƬ��·��
	loadimage(&enemyimg, _T("H:\\CodeResource\\PlaneFight_study\\images\\enemy1.png"));
	loadimage(&bkimg, _T("H:\\CodeResource\\PlaneFight_study\\images\\bk3.png"), swidth, sheight * 2);		//����һ�θ�ʽ��
	loadimage(&bulletimg, _T("H:\\CodeResource\\PlaneFight_study\\images\\bullet1.png"));

	BK bk = BK(bkimg);				//ʵ�����ı�������
	Hero hp = Hero(heroimg);			//ʵ����һ��Ӣ�۶��󣬴���Ӣ��ͼƬ

	vector<Enemy*> es;
	for (int i = 0; i < 4; i++)
	{
		es.push_back(new Enemy(enemyimg, abs(rand()) % (swidth - enemyimg.getwidth())));			//��ʼ��һ���л�,����Ϊ�л�����Ƭ,�������һ��x����,��ע���ұ���Ҫ�����͵л����һ���Ŀ��
	}

	//ͼ�ν���

	while (is_play)								//��Ϸ����ѭ��
	{
		BeginBatchDraw();						//��ֹ����

		bk.Show();								//���õĺ���
		Sleep(6);								//
		flushmessage();							//ˢ����Ϣ�Ļ��������Ի�����µ���Ϣ
		Sleep(2);								//sleepһ���ٻ����Ϣ
		hp.Control();
										
		hp.Show();								//������ҵķɻ�

		//Sleep(16);								//����ÿ��60֡���ٶ�

		auto it = es.begin();			//����һ��ָ���������������ĳ�ʼλ��	
		while (it != es.end())			//��û��ͷ,���������ѭ��
		{
			if (!(*it)->Show())			//��������false,��˵���ɳ�ȥ��,Ҫ�����������
			{
				delete(*it);			//ɾ��new������ָ��
				/*es.erase(it);*/		//��Ƶ����һ�������������
				it = es.erase(it);			//ʹ�����ٺ���,����itָ�����һ��ֵ,����û��ѭ����
			}
			else				//else���ڽ��һ������
				it++;
						//���û������,���ָ�����++,ָ����һ��
		}

		for (int i = 0; i < 4 - es.size(); i++)				//�Եл���̬��������
		{
			es.push_back(new Enemy(enemyimg, abs(rand()) % (swidth - enemyimg.getwidth())));			//��ӵл�
		}
		EndBatchDraw();
	}
	return true;
}

int main() {
	//easyX��ʼ��
	initgraph(swidth, sheight, EX_NOMINIMIZE | EX_SHOWCONSOLE);
	bool is_live = true;		//��Ϸ�Ƿ��ڽ���
	while (is_live)				//�����Ϸ�ڽ��У����������
	{
		Welcome();				//�򿪿�ʼ����
		//��Ϸ���溯��
		is_live = Play();		//��Ϸ��ѭ���Ƿ�Ϊtrue�������ص�play()���Ϊfalse,��is_liveΪfalse,���˳���ѭ����������Ϸ����
	}

	return 0;
}