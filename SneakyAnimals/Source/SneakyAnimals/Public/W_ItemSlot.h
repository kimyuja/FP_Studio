// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_ItemSlot.generated.h"

/**
 *
 */
UCLASS()
class SNEAKYANIMALS_API UW_ItemSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize();


private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	//class UTexture2D* itemImg;
	class UButton* btn;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	//class UTexture2D* itemImg;
	class UImage* img;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UTexture2D* itemImg;

	// 버튼인덱스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 itemSlot;

	// 각 슬롯 위로 드래그 할 때 실행될 버튼 스타일
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FButtonStyle EnabledButtonStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FButtonStyle DragOverStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FButtonStyle DefaultButtonStyle;

};
