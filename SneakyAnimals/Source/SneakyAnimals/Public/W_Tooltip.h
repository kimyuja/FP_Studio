// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Tooltip.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_Tooltip : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void Setup();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UTexture2D* Image;
	
	UPROPERTY(EditAnywhere, Category = "MySettings")
	FText Tooltip_Title;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FText Tooltip_Description;

protected:
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class USizeBox* Image_SizeBox;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* Tooltip_Image;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* Title_Text;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* Tooltip_Description_Text;

	void Set_Image();

};
