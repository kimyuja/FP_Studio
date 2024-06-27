// Fill out your copyright notice in the Description page of Project Settings.


#include "W_CustomMap.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include <ItemComponent.h>
#include "LineStructure.h"
#include <../../../../../../../Source/Runtime/Experimental/ChaosCore/Public/Chaos/Array.h>

void UW_CustomMap::NativeConstruct()
{
	Super::NativeConstruct();

	if (itemComponent)
	{
		itemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("itemComponent"));
	}

	/*if (!itemComponent)
    {
        itemComponent = NewObject<UItemComponent>(this, TEXT("ItemComponent"));
        itemComponent->RegisterComponent();
    }*/
	
	if (gridBorder && gridCanvasPanel)
	{
		gridBorder->SetContent(gridCanvasPanel);
		
		UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(gridBorder->Slot);

		if (canvasSlot)
		{
			/*float sizeX = itemComponent->columns * tileSize;
			float sizeY = itemComponent->rows * tileSize;
			canvasSlot->SetSize(FVector2D(sizeX, sizeY));*/
			canvasSlot->SetSize(FVector2D(160.f, 160.f));

			UE_LOG(LogTemp, Warning, TEXT("colums : %d"), itemComponent->columns);
			UE_LOG(LogTemp, Warning, TEXT("rows : %d"), itemComponent->rows);
			UE_LOG(LogTemp, Warning, TEXT("tileSize : %f"), tileSize);
		}
	}

}

bool UW_CustomMap::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!gridBorder) return false;

	gridBorder->SetPadding(0.0f);
	return true;
}

void UW_CustomMap::InitializeWidget(UItemComponent* ItemComponent, float Tilesize)
{
	/*UE_LOG(LogTemp, Warning, TEXT("initialized widget to set size"));

	UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(gridBorder->Slot);

	if (canvasSlot)
	{
		float sizeX = itemComponent->columns * tileSize;
		float sizeY = itemComponent->rows * tileSize;
		canvasSlot->SetSize(FVector2D(sizeX, sizeY));
	}*/

}

void UW_CustomMap::CreateLineSegments()
{
	CreateVerticalLine();
	CreateHorizantalLine();

}

void UW_CustomMap::CreateVerticalLine()
{
	float X;	// 로컬변수

	for (int32 i = 0; i < itemComponent->columns; ++i) {
		X = tileSize * i;

		ULineStructure newLine;
		newLine.start = FVector2D(X, 0.0f);
		newLine.end = FVector2D(X, tileSize * itemComponent->rows);

		lines.Add(&newLine);
	}
}

void UW_CustomMap::CreateHorizantalLine()
{
	float Y;	// 로컬변수

	for (int32 i = 0; i < itemComponent->rows; ++i) {
		Y = tileSize * i;

		ULineStructure newline;
		newline.start = FVector2D(0, Y);
		newline.end = FVector2D(tileSize * itemComponent->columns, Y);

		lines.Add(&newline);
	}
}

//int32 UW_CustomMap::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
//{
//	int32 RetLayerId = Super::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
//
//	FVector2D TopLeft = AllottedGeometry.GetLocalPositionAtCoordinates(FVector2D(0.0f, 0.0f));
//
//	// Lines 배열을 순회하며 그리기 작업 수행
//	
//
//	return RetLayerId;
//}
