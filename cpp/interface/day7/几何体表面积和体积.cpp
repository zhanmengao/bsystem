#include <iostream>
using namespace std;
#include<math.h>

class geometry{
public:
	virtual float getVolume() = 0;
	virtual float getSize() = 0;
	virtual ~geometry()
	{
		cout << "������������" << endl;
	}
};
class Cube : public geometry
{
public:
	Cube(float Long, float width,float height)
	{
		this->Long = Long;
		this->Width = width;
		this->Height = height;
		Volume = Long*width*height;
		Size = 2 * (width + Long + Height);
	}
	virtual ~Cube()
	{
		cout << "������������" << endl;
	}
	virtual float getVolume()
	{
		return this->Volume;
	}
	virtual float getSize()
	{
		return this->Size;
	}
private:
	float Long;
	float Width;
	float Height;
	float Volume;
	float Size;
};

class Sphere : public geometry
{
public:
	Sphere(float r)
	{
		this->r = r;
		Volume = 3.14f*3.14f*r*r;
		Size = 4 * 3.14f*r*r;
	}
	virtual float getVolume()
	{
		return this->Volume;
	}
	virtual float getSize()
	{
		return this->Size;
	}
	virtual ~Sphere()
	{
		cout << "������������" << endl;
	}
private:
	float r;
	float Volume;
	float Size;
};

float Size(geometry &ge)
{
	return ge.getSize();
}
float Volume(geometry *ge)
{
	return ge->getVolume();
}
void Delete(geometry *cube)
{
	delete cube;
}

void main2()
{
	geometry *ge = NULL;
	Cube *cube = new Cube(1,2,3);
	ge = cube;
	cout <<"���" <<Volume(ge) << endl;;
	cout<<"�����"<<Size(*ge)<<endl;
	Delete(cube);

	Sphere *sphere = new Sphere(3);
	ge = sphere;
	cout << "���" << Volume(ge) << endl;;
	cout << "�����" << Size(*ge) << endl;
	Delete(sphere);
}