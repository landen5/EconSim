#pragma once


class Sim {
private:
	float simSpeed;
public:
	Sim();
	void Update(float elapsed);
	void increaseSpeed();
	void decreaseSpeed();
	float getSimSpeed();

};
