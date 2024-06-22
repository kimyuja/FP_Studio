// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_SmallProfile.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_SmallProfile : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category="MySettings", meta = (BindWidget))
	class UImage* ProfilePicture_Image;
	
	UPROPERTY(VisibleAnywhere, Category="MySettings", meta = (BindWidget))
	class UTextBlock* Username_Text;

	UFUNCTION()
	void Refresh_Widget();

protected:
	virtual void NativeConstruct() override;
};
