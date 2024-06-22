// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_AvailableAvatar.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_AvailableAvatar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* Avatar_Image;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UButton* Button;

	UPROPERTY(EditAnywhere, Category = "MySettings", BlueprintReadWrite)
	class UTexture2D* Avatar;

	UPROPERTY(EditAnywhere, Category = "MySettings", BlueprintReadWrite)
	class UUserWidget* ParentWidget;

	UPROPERTY(EditAnywhere, Category = "MySettings", BlueprintReadWrite)
	FName Avatar_Name;

	UFUNCTION()
	void OnClicked();

protected:
	virtual void NativeConstruct() override;

};
