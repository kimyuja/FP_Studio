// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataStructure.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/UserWidget.h>
#include "FL_General.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UFL_General : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFL_General();

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	static bool Save_UserProfile(const FStructure_UserProfile& S_UserData);

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	static FUserProfileResult Get_UserProfile();

	UFUNCTION(BlueprintCallable, Category = "User Widgets")
	static class UW_LoadingScreen* Create_LoadingScreen(UObject* WorldContextObject, UTexture2D* LoadingScreenImage, FText LoadingScreenFeedbackText);
	
	UFUNCTION(BlueprintCallable, Category = "User Widgets")
	static class UW_PopUp* Create_PopUp(UObject* WorldContextObject, FText PopUp_Message, FText Left_Btn_Text, bool bContains_Right_Button, FText Right_Btn_Text);
	
	UFUNCTION(BlueprintCallable, Category = "Session")
	static void Set_SessionInfo(UObject* WorldContextObject, FString ServerName, int32 MaxPlayers);

private:
    static TSubclassOf<UUserWidget> WB_LoadingScreen_bp;
	static TSubclassOf<UUserWidget> WB_PopUp_bp;
};
