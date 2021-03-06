#include<iostream>
#include <math.h>
#include<Vector>
#include<SFML/Window.hpp>
#include <SFML/Graphics.hpp>



/*****************************************************************************************************

Instructions:
Run the code
	The <- and -> arrow keys turn the camera
	W A S D Keys are to move
	Space Bar randomly generates a new 'map'

	This file contains a lot of edits and commented out code as it is just for educational purposes

*****************************************************************************************************/






float DegreeorRadian(bool degrad, float angle) {
	//Utility tool to convert degree to radian and vise versa
	if (degrad == true) {
		//degree to radian
		angle *= 0.0174533;
	}
	else {
		//radian to degree
		angle /= 0.0174533;
	}
	return angle;
}


sf::Vertex xyFromAngleHypotenuse(float hypotenuse, float angle) {
	sf::Vertex xy;
	angle = DegreeorRadian(true, angle);
	xy.position.x = cosf(angle) * hypotenuse;
	xy.position.y = sinf(angle) * hypotenuse;
	return xy;
}

float AnglePointnMouse(sf::Vertex p1) {
	sf::Vector2i p2 = sf::Mouse::getPosition();
	return atanf((p2.y - p1.position.y) / (p2.x - p1.position.x));
}

class Boundary : public sf::Transformable, public sf::Drawable {
private:
	sf::VertexArray points;
public:
	Boundary() {
		//std::cout << "\nHello Boundary";
		points.resize(2);
		this->setOrigin(0, 0);
		points.setPrimitiveType(sf::PrimitiveType::Lines);
		points[0].position = sf::Vector2f(0.0f, 0.0f);
		points[0].color = sf::Color::White;
		points[1].position = sf::Vector2f(200.0f, 0.0f);
		points[1].color = sf::Color::White;

		this->setPosition(100, 100);
		this->setRotation(90);
	}
	void setBoundary(float x, float y, float size, float angle) {
		points.resize(2);
		this->setOrigin(0, 0);
		points.setPrimitiveType(sf::PrimitiveType::Lines);
		points[0].position = sf::Vector2f(0.0f, 0.0f);
		points[0].color = sf::Color::White;
		points[1].position = sf::Vector2f(size, 0.0f);
		points[1].color = sf::Color::White;

		this->setPosition(x, y);
		this->setRotation(angle);
	}

	sf::VertexArray getPoints() {
		return points;
	}


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform = getTransform();
		target.draw(points, states);
	}
};


class Ray : public sf::Transformable, public sf::Drawable {
private:
	//I need to define 2 points
	//A vertex array
	sf::VertexArray points;
	sf::Vector2f PoI; //point of Intersection
	sf::VertexArray LoI; //Line of Intersection
	sf::VertexArray PoIVertex;
	sf::Color ColP;
	sf::Color ColL;
	sf::Color ColPoI;
	bool isLine;
public:
	Ray() {
		//std::cout << "Hello Ray";
		this->PoIVertex.resize(0);
		this->LoI.resize(0);
		this->LoI.setPrimitiveType(sf::PrimitiveType::Lines);
		this->points.resize(2);
		this->points.setPrimitiveType(sf::PrimitiveType::Lines);
		this->points[0].position = sf::Vector2f(0.0f, 0.0f);
		this->points[1].position = sf::Vector2f(40.0f, 0.0f);
		this->points[0].color = sf::Color::Color(255, 255, 255, 25);
		this->points[1].color = sf::Color::Color(255, 255, 255, 25);

	}
	void setRayParticle(float x, float y, float angle) {
		this->LoI.resize(0);
		this->LoI.setPrimitiveType(sf::PrimitiveType::Lines);
		this->points.resize(2);
		this->points.setPrimitiveType(sf::PrimitiveType::Lines);
		this->PoIVertex.setPrimitiveType(sf::PrimitiveType::Points);
		this->points[0].position = sf::Vector2f(0.0f, 0.0f);
		this->points[1].position = sf::Vector2f(20.0f, 0.0f);
		this->points[0].color = sf::Color::Color(255, 255, 255, 25);
		this->points[1].color = sf::Color::Color(255, 255, 255, 25);

		this->setPosition(x, y);
		this->setRotation(angle);
	}

	void setColors(sf::Color p, sf::Color poi, sf::Color loi) {
		ColP = p;
		ColL = loi;
		ColPoI = poi;
	}


	sf::Vector2f& getPoI() {
		return PoI;
	}
	void setLoI(sf::Vector2f poi) {
		LoI.resize(0);
		PoIVertex.resize(0);
		LoI.append(this->getPosition());
		LoI.append(poi);
		LoI[0].color = ColL;
		LoI[1].color = ColL;
		PoIVertex.append(poi);
		PoIVertex[0].color = ColPoI;
	}


	void Checkup(Boundary* walls, int size) {
		sf::VertexArray wallpoints;
		float x1, x2, y1, y2, x3, x4, y3, y4;
		float wallH;
		float wallangle;
		sf::Vertex wall1;
		float rayangle = this->getRotation();
		float rayH;
		sf::Vertex ray1;
		float Record = FLT_MAX;
		float current = FLT_MAX;
		sf::Vector2f recordPoint;
		int recordI;
		for (int i = 0; i < size; i++) {
			wallpoints = walls[i].getPoints();
			wallangle = walls[i].getRotation();
			wallH = sqrtf(powf(wallpoints[1].position.x - wallpoints[0].position.x, 2) + powf(wallpoints[1].position.y - wallpoints[0].position.y, 2));
			rayH = sqrtf(powf(this->points[1].position.x - this->points[0].position.x, 2) + powf(this->points[1].position.y - this->points[0].position.y, 2));
			wall1 = xyFromAngleHypotenuse(wallH, wallangle);
			ray1 = xyFromAngleHypotenuse(rayH, rayangle);
			x1 = walls[i].getPosition().x;
			x2 = walls[i].getPosition().x + wall1.position.x;
			y1 = walls[i].getPosition().y;
			y2 = walls[i].getPosition().y + wall1.position.y;
			x3 = this->getPosition().x;
			x4 = this->getPosition().x + ray1.position.x;
			y3 = this->getPosition().y;
			y4 = this->getPosition().y + ray1.position.y;

			//wikipedia.org/wiki/Line%E2%80%93line_intersection
			//we first calculate denominator - its common;
			float denominator = (((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)));


			if (denominator == 0) {
				//points[0].color = (sf::Color::White);
				//points[1].color = (sf::Color::White);
				this->isLine = false;
				continue;
			}

			float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
			float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denominator;

			if (t > 0 && t < 1 && u>0) {

				//points[0].color = (sf::Color::Red);
				//points[1].color = (sf::Color::Red);
				this->isLine = true;
				this->Check(walls[i]);
				current = sqrtf((powf(PoI.x - this->getPosition().x, 2) + powf((PoI.y - this->getPosition().y), 2)));
				if (current < Record) {
					//recordI = i;
					Record = current;
					recordPoint = sf::Vector2f(PoI.x, PoI.y);

				}
			}
			else {
				this->isLine = false;
				continue;
			}
		}
		PoI.x = recordPoint.x;
		PoI.y = recordPoint.y;
		this->setLoI(PoI);
		this->isLine = true;
	}
	void Check(Boundary wall) {
		LoI.resize(0);
		sf::VertexArray wallpoints = wall.getPoints();
		float x1, x2, y1, y2, x3, x4, y3, y4;
		float wallangle = wall.getRotation();
		float wallH;
		wallH = sqrtf(powf(wallpoints[1].position.x - wallpoints[0].position.x, 2) + powf(wallpoints[1].position.y - wallpoints[0].position.y, 2));
		float rayangle = this->getRotation();
		float rayH;
		rayH = sqrtf(powf(this->points[1].position.x - this->points[0].position.x, 2) + powf(this->points[1].position.y - this->points[0].position.y, 2));
		sf::Vertex wall1 = xyFromAngleHypotenuse(wallH, wallangle);
		sf::Vertex ray1 = xyFromAngleHypotenuse(rayH, rayangle);

		x1 = wall.getPosition().x;
		x2 = wall.getPosition().x + wall1.position.x;
		y1 = wall.getPosition().y;
		y2 = wall.getPosition().y + wall1.position.y;
		x3 = this->getPosition().x;
		x4 = this->getPosition().x + ray1.position.x;
		y3 = this->getPosition().y;
		y4 = this->getPosition().y + ray1.position.y;



		float denominator = (((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)));

		float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
		float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denominator;

		if (t > 0 && t < 1 && u>0) {
			//points[0].color = (sf::Color::Red);
			//points[1].color = (sf::Color::Red);

			PoI.x = x1 + (t * (x2 - x1));
			PoI.y = y1 + (t * (y2 - y1));
			this->setLoI(PoI);
			//LoI.append(this->getPosition());
			//LoI.append(PoI);
			//LoI.setPrimitiveType(sf::PrimitiveType::Lines);
		}

	}

	//look at function only works when x2 is in front of thisposition.x
	//fixed it useing if statement
	void lookat(float x2, float y2) {
		float angle;
		float o = y2 - this->getPosition().y;
		float a = x2 - this->getPosition().x;
		angle = atanf((o / a));
		if (x2 >= this->getPosition().x)
			angle = DegreeorRadian(0, angle);
		else
			angle = DegreeorRadian(0, angle) - 180;
		this->setRotation(angle);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{

		if (isLine && (LoI[1].position.x != 0 && LoI[1].position.y != 0)) {
			target.draw(LoI, states);
			target.draw(PoIVertex, states);
		}
		//states.transform = getTransform();
		//target.draw(points, states);
		//target.draw(LoI, states);
	}
};

class Particle : public sf::Transformable, public sf::Drawable {
private:
	Ray* rays;
	std::vector<float> Scene;
	sf::RectangleShape* SceneRect;
	int sceneW;
	int sceneH;
	int count;
public:
	Particle(float w, float h) {
		count = 60;
		this->setPosition(w, h);
		sceneW = w * 2;
		sceneH = h * 2;
		SceneRect = new sf::RectangleShape[count];
		rays = new Ray[count];
		for (int i = -count / 2; i < count / 2; i++) {
			rays[i + count / 2].setRayParticle(this->getPosition().x, this->getPosition().y, i);
			//SceneRect[i].setOrigin(SceneRect[i].getOrigin().x, sceneH/2);
		}
	}

	void look(Boundary* walls, int size) {
		Scene.resize(0);
		float seenati;
		float maxval;
		float factor;
		//float siz;
		//float poz;
		for (int i = 0; i < count; i++) {
			rays[i].Checkup(walls, size);
			Scene.push_back(sqrt(powf((this->getPosition().x - rays[i].getPoI().x), 2) + powf((this->getPosition().y - rays[i].getPoI().y), 2))); //appends count ammount of intersection points
			seenati = Scene.at(i);
			maxval = sqrtf(pow(sceneW, 2) + pow(sceneH, 2));
			factor = (maxval - seenati) / maxval;
			//factor = (1 / (Scene.at(i) / (sceneW * sqrtf(2))));
			//siz = sceneH /factor;
			//poz = (sceneH-siz)/2;

			//std::cout << maxval<<std::endl;
			//std::cout << "\ni" << i << " size "<<siz << " Position "<< poz<< std::endl;
			//SceneRect[i].setPosition((i * (sceneW / count)) + sceneW, sceneH / 2);
			//SceneRect[i].setPosition((i * (sceneW / count)) + sceneW, 0);
			//SceneRect[i].setSize(sf::Vector2f(sceneH, ));
			//SceneRect[i].rotate(90);
			//SceneRect[i].move((sceneW / count), 0);

			//SceneRect[i].setSize(sf::Vector2f ((sceneW / count), siz));
			SceneRect[i].setSize(sf::Vector2f((sceneW / count), sceneH * factor * factor));
			SceneRect[i].setPosition(sf::Vector2f((i * (sceneW / count)) + sceneW, (sceneH - (SceneRect[i].getSize().y)) / 2));
			SceneRect[i].setFillColor(sf::Color::Color(255, 255, 255, 255 * (factor * factor))); // red, blue, green, alpha //all values are %255

		}
	}

	void update(float x, float y) {
		this->setPosition(x, y);
		for (int i = 0; i < count; i++) {
			rays[i].setPosition(x, y);
		}
	}
	void updateRelative(float x, float y) {
		//up - 0
		//down - 180
		//right - 90
		//left - 270
		//we need to find the hupotenuse, which is equal to distance we need to move,
		float magnitude = sqrtf(pow(x, 2) + pow(y, 2));
		sf::Vertex xy;
		sf::Vertex xy2;
		sf::Vertex prev;
		prev = this->getPosition();
		int prot = this->getRotation();
		float magni;
		if (x > y && y == 0) {//right
			xy = xyFromAngleHypotenuse(magnitude, this->getRotation() + 90);
			magni = sqrtf(pow(xy.position.x - prev.position.x, 2) + pow(xy.position.y - prev.position.y, 2));

			xy2 = xyFromAngleHypotenuse(magni, this->getRotation() + 90);

			//xy2 = xyFromAngleHypotenuse(magnitude, this->getRotation());
			//this->move(xyFromAngleHypotenuse(xy.position.x, 0).position.x, xyFromAngleHypotenuse(xy.position.y, 0).position.y);
			this->move(xy.position.x, xy.position.y);
		}
		//if (x < y && y == 0) {//left
		if (x < y && y == 0) {//left
			xy = xyFromAngleHypotenuse(magnitude, this->getRotation() + 270);
			magni = sqrtf(pow(xy.position.x - prev.position.x, 2) + pow(xy.position.y - prev.position.y, 2));
			xy2 = xyFromAngleHypotenuse(magni, this->getRotation() + 270);
			//xy2 = xyFromAngleHypotenuse(magnitude, this->getRotation());
			//this->move( xyFromAngleHypotenuse(xy.position.x, 180).position.x, xyFromAngleHypotenuse(xy.position.y, 0).position.y);
			//this->move(xy.position.x, xy.position.y);
			this->move(xy.position.x, xy.position.y);
		}
		if (y > x && x == 0) {//down
			xy = xyFromAngleHypotenuse(magnitude, this->getRotation() + 180);
			this->move(xy.position.x, xy.position.y);
		}
		if (y < x && x == 0) {//up
			xy = xyFromAngleHypotenuse(magnitude, this->getRotation() + 0);
			this->move(xy.position.x, xy.position.y);
		}
		for (int i = 0; i < count; i++) {
			rays[i].setPosition(this->getPosition().x, this->getPosition().y);
		}
	}

	void ROT(int angle) {
		this->rotate(angle);
		for (int i = 0; i < count; i++) {
			rays[i].rotate(angle);
		}
	}

	void face(float x2, float y2) {
		float angle;
		float o = y2 - this->getPosition().y;
		float a = x2 - this->getPosition().x;
		angle = atanf((o / a));
		if (x2 >= this->getPosition().x)
			angle = DegreeorRadian(0, angle);
		else
			angle = DegreeorRadian(0, angle) - 180;
		//this->ROT();
		this->setRotation(angle);
		for (int i = 0; i < count; i++) {
			rays[i].lookat(x2, y2);
			rays[i].setRayParticle(this->getPosition().x, this->getPosition().y, this->getRotation() + (i - count / 2));
		}
	}

	void automate(int width, int height) {
		srand(time(NULL));
		this->move((-5 + (rand() % 11)), -5 + (rand() % 11));
		sf::Vector2f Pos = getPosition();

		if (Pos.x > width - 30) {
			this->setPosition(width - 30, Pos.y);
			//this->move(-10, 0);
		}
		if (Pos.y > height - 30) {
			this->setPosition(Pos.x, height - 30);
			//this->move(0,-10);
		}
		if (Pos.x < 30) {
			this->setPosition(30, Pos.y);
			//this->move(10, 0);
		}
		if (Pos.y < 30) {
			this->setPosition(Pos.x, 30);
			//this->move(0, 10);
		}

		if (Pos.x >= width - 30) {
			this->move(-1, 0);
		}
		if (Pos.y >= height - 30) {
			this->move(0, -1);
		}
		if (Pos.x <= 30) {
			this->move(1, 0);
		}
		if (Pos.y <= 30) {
			this->move(0, 1);
		}

		for (int i = 0; i < count; i++) {
			rays[i].setPosition(this->getPosition());
		}
	}

	void setRayColors(sf::Color p, sf::Color poi, sf::Color loi) {
		for (int i = 0; i < count; i++) {
			rays[i].setColors(p, poi, loi);
		}
	}

	int getSceneCount() {
		return count;
	}
	sf::RectangleShape* getSceneRect() {
		return SceneRect;
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (int i = 0; i < count; i++) {
			rays[i].draw(target, states);
		}
	}
};


int main() {
	srand(time(NULL));
	int width, height;
	width = 850;
	height = 850;
	int wallcount = 10 + 4;
	sf::RenderWindow window(sf::VideoMode(width * 2, height), "First Window!", sf::Style::Default);

	window.setVerticalSyncEnabled(true);

	Particle particle(width / 2, height / 2);

	particle.setRayColors(sf::Color::Color(255, 255, 255, 255), sf::Color::Color(255, 0, 0, 127), sf::Color::Color(255, 0, 0, 127));


	Boundary* walls;
	walls = new Boundary[wallcount];
	for (int i = 0; i < wallcount - 4; i++) {

		walls[i].setBoundary(rand() % width, rand() % height, rand() % 100, rand() % 275);
		std::cout << "\nwall " << i << " p1: " << walls[i].getPoints()[0].position.x + walls[i].getPosition().x << ", " << walls[i].getPoints()[0].position.y + walls[i].getPosition().y;
		std::cout << "\nwall " << i << " p2: " << walls[i].getPoints()[1].position.x + walls[i].getPosition().x << ", " << walls[i].getPoints()[1].position.y + walls[i].getPosition().y;

		while (walls[i].getPoints()[1].position.x + walls[i].getPosition().x > width) {
			std::cout << "\nWALLLLLLLL = " << walls[i].getPoints()[1].position.x + walls[i].getPosition().x << std::endl;
			walls[i].setPosition(walls[i].getPosition().x - 100, walls[i].getPosition().y);
			std::cout << "\nWALLLLLLLL = " << walls[i].getPoints()[1].position.x + walls[i].getPosition().x;
		}
	}
	walls[wallcount - 4].setBoundary(0, 1, width, 0);
	walls[wallcount - 3].setBoundary(width, 0, height, 90);
	walls[wallcount - 2].setBoundary(width, height, width, 180);
	walls[wallcount - 1].setBoundary(0, height, height, 270);



	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::MouseMoved)
			{
				//particle.update(event.mouseMove.x, event.mouseMove.y);
				//particle.face(event.mouseMove.x, event.mouseMove.y);
			}
			else if (event.type == sf::Event::KeyPressed) {
				/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
					particle.update(particle.getPosition().x - 5, particle.getPosition().y);
					//particle.ROT(2);

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					particle.update(particle.getPosition().x + 5, particle.getPosition().y);
					//particle.ROT(-2);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					particle.update(particle.getPosition().x, particle.getPosition().y - 5);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					particle.update(particle.getPosition().x, particle.getPosition().y + 5);
				}*/
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					for (int i = 0; i < wallcount - 4; i++) {

						walls[i].setBoundary(rand() % width, rand() % height, rand() % 100, rand() % 275);

					}
				}

			}
			else if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			//particle.update(particle.getPosition().x - 5, particle.getPosition().y);
			particle.updateRelative(-5, 0);
			//particle.ROT(-2);

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			//particle.update(particle.getPosition().x + 5, particle.getPosition().y);
			particle.updateRelative(5, 0);
			//particle.ROT(2);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			//particle.update(particle.getPosition().x - 5, particle.getPosition().y);
			//particle.updateRelative(- 5, 0);
			particle.ROT(-2);

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			//particle.update(particle.getPosition().x + 5, particle.getPosition().y);
			//particle.updateRelative(5, 0);
			particle.ROT(2);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			//particle.update(particle.getPosition().x, particle.getPosition().y - 5);
			particle.updateRelative(0, -5);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			//particle.update(particle.getPosition().x, particle.getPosition().y + 5);
			particle.updateRelative(0, 5);
		}


		//particle.face(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		particle.look(walls, wallcount);
		for (int i = 0; i < wallcount; i++) {
			window.draw(walls[i]);
		}
		window.draw(particle);

		for (int i = 0; i < particle.getSceneCount(); i++) {
			window.draw(particle.getSceneRect()[i]);
		}

		window.display();

	}


	return 0;
}