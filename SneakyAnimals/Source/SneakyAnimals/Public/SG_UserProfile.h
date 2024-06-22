// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DataStructure.h"
#include "SG_UserProfile.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API USG_UserProfile : public USaveGame
{
	GENERATED_BODY()
public:
    USG_UserProfile();
    
    // 저장할 변수를 선언, SaveGame에서는 * 포인터 사용 금지! (이유 : 직렬화가 안 될 수 있어서.)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MySettings")
    FStructure_UserProfile S_UserProfile;
	
};
