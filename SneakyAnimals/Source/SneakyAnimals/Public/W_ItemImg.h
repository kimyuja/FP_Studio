// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <../../../../../../../Source/Runtime/Core/Public/Delegates/DelegateCombinations.h>
#include "W_ItemImg.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoved, UItemObject*, ItemObject);
/**
 *
 */
UCLASS()
class SNEAKYANIMALS_API UW_ItemImg : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;


	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USizeBox* backgroundSizeBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* backgroundBorder;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* itemImage;

public:

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
    class UItemObject* itemObject;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
	FVector2D size;
	
	UPROPERTY(BlueprintReadWrite, Category = "Item")
	float tileSize;

	UFUNCTION(BlueprintCallable, Category = "Item")
	void Refresh();

	FTimerHandle timerHandle;

	UFUNCTION(BlueprintCallable, Category = "Binding")
	FSlateBrush GetIconImage() const;

	// 델리게이트 인스턴스 선언
	FOnRemoved OnRemoved;

	UFUNCTION()
	void RemoveItem(class UItemObject* _ItemObject);

	UPROPERTY()
	class UW_CustomMap* customMapComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> itemSlotWidget;

};
