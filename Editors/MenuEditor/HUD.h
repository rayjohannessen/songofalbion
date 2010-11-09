#pragma once

class CHUD
{
	CHUD(void) {}
	~CHUD(void) {}
	CHUD(const CHUD&);
	CHUD& operator= (const CHUD&);
public:

	static CHUD* GetInstance();

	void Init();
	void Shutdown();

	void Render();
	void Update(float fElapsed);
};
