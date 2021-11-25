// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectTedGameMode.h"
#include "ProjectTedCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectTedGameMode::AProjectTedGameMode()
{
	// set default pawn class to our Blueprinted character
	//注意：如果在工程的ContentBrowser中更改了文件目录和对应资产的名称，这里不会同步更改。那么每次运行工程，都会弹窗提示找不到以下路径中的资源这一错误。因是纯字符串，所以需要手动更新。
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Ted25DSideScrollerCPP/Blueprints/Ted25DSideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
