// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DataStructure.h"
#include "SG_PlayerAppearance.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API USG_PlayerAppearance : public USaveGame
{
	GENERATED_BODY()

public:
    //USG_PlayerAppearance();
    
    // ������ ������ ����, SaveGame������ * ������ ��� ����! (���� : ����ȭ�� �� �� �� �־.)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MySettings")
    FStructure_Player_Appearance S_PlayerAppearance;
};
