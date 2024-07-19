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
	UW_Lobby_PlayerList(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* PlayerCount_Text;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UScrollBox* PlayerList;

	UFUNCTION()
	void Update_Widget(TArray<FStructure_ConnectedPlayer> _Connected_Players);

	UPROPERTY(EditAnywhere, Category="References")
	TSubclassOf<class UW_Lobby_PlayerList_Item> Lobby_PlayerList_Item_bp;

	UPROPERTY(EditAnywhere, Category = "References")
	class UW_Lobby_PlayerList_Item* Lobby_PlayerList_Item_inst;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    TArray<FStructure_ConnectedPlayer> Connected_Players;

private:
	void Update_PlayerList();
	void Update_PlayerCount();
};
