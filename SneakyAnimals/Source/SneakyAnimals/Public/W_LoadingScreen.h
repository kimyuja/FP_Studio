// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_LoadingScreen.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_LoadingScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* Background_Image;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* Feedback_Text;
	
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UTexture2D* LoadingScreen_Image;
	
	UPROPERTY(EditAnywhere, Category = "MySettings")
	FText LoadingScreen_FeedbackText;

protected:
	virtual void NativeConstruct() override;
};
