// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDrag.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UItemDrag : public UDragDropOperation
{
	GENERATED_BODY()

public:
	// 드래그 앤 드롭 작업을 수행하고 있는 아이템바 또는 레벨의 인덱스 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotIndex;
	// Editable -> true
	// Expose on Spawn -> true 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* itemImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool fromItemBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* DraggedItem;
	
};
