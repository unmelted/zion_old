#pragma once
#include "SystemAPI.h"
#include <memory>	// for smart pointer
#include <string>

class FdrPlayer {
public:
	class Player;
public:
	typedef struct {
		HANDLE hWnd;
		std::string url;
	} Param;

	FdrPlayer();
	~FdrPlayer();
	int Open(Param& param);
	int Close();
	int Start();
	int Stop();

private:
	FdrPlayer::Player* m_hPlayer;
	
	Param _param;
	std::unique_ptr<Player> mImplPlayer;
};
