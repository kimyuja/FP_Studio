// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStructure.h"
#include "Blueprint/UserWidget.h"
#include "W_ServerBrowserItem.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_ServerBrowserItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* LobbyName_Text;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* AmountOfPlayers_Text;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* Ping_Text;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* JoinSession_Btn;

	UPROPERTY(BlueprintReadOnly, Category = "Session")
    FSessionInfo SessionToJoin;

	void Setup(const struct FSessionInfo& info);

	UFUNCTION()
	void OnMyJoinSession_BtnClicked();

protected:
	virtual void NativeConstruct() override;

private:
	UTexture2D* T_Color4_Normal;
};
