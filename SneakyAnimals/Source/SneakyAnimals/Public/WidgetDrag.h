// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "WidgetDrag.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UWidgetDrag : public UDragDropOperation
{
	GENERATED_BODY()

public:
	// 현재 드래그 하는 위젯
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* WidgetToDrag;
	// Editable -> true
	// Expose on Spawn -> true 
	
	// 마우스 클릭할 때마다 드래그하는 위치 오프셋하고 원하는 위젯 드래그
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D DropWindowOffset;



};
