// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_DraggedItem.generated.h"

/**
 *
 */
UCLASS()
class SNEAKYANIMALS_API UW_DraggedItem : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	// √ ±‚»≠
	virtual bool Initialize();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	//class UTexture2D* itemImg;
	class UImage* itemImg;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* itemCost;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* itemCount;


};
