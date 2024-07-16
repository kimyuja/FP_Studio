// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataStructure.h"
#include "W_Lobby_PlayerList.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_Lobby_PlayerList : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* PlayerCount_Text;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UScrollBox* PlayerList;

	UFUNCTION()
	void Update_Widget(TArray<FStructure_ConnectedPlayer> _Connected_Players);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    TArray<FStructure_ConnectedPlayer> Connected_Players;

private:
	void Update_PlayerList();
	void Update_PlayerCount();
};
