// Fill out your copyright notice in the Description page of Project Settings.


#include "W_CustomMap.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include <ItemComponent.h>
#include "LineStructure.h"
#include <../../../../../../../Source/Runtime/Experimental/ChaosCore/Public/Chaos/Array.h>
#include "Slate/WidgetTransform.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h>
#include "Rendering/DrawElements.h"
#include "Layout/Geometry.h"
#include "TileStructure.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <UITestCharacter.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanel.h>
#include "W_ItemImg.h"
#include <../../../../../../../Source/Runtime/Core/Public/Delegates/Delegate.h>

UW_CustomMap::UW_CustomMap(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UW_CustomMap::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!gridBorder) return false;

	gridBorder->SetPadding(0.0f);


	return true;
}

void UW_CustomMap::NativeConstruct()
{
	Super::NativeConstruct();

	
	/*
	if (itemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("itemComponent is not nullptr"));
	}*/

	if (gridBorder && gridCanvasPanel)
	{
		gridBorder->SetContent(gridCanvasPanel);
	}

	Invalidate(EInvalidateWidget::LayoutAndVolatility);

	GetWorld()->GetTimerManager().SetTimer(DrawGridLineTimerHandle, this, &UW_CustomMap::DrawGridLine, 0.1f, false);

	UE_LOG(LogTemp, Warning, TEXT("itemComponent->columns : %d, itemComponent->rows : %d"), itemComponent->columns, itemComponent->rows);

	// DrawGridLine();
}

int32 UW_CustomMap::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 RetLayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	FVector2D topLeft = GetGridBorderTopLeft();

	// Draw each line
	for (const ULineStructure* Line : lines)
	{
		FSlateDrawElement::MakeLines(
			OutDrawElements,
			RetLayerId,
			AllottedGeometry.ToPaintGeometry(),
			TArray<FVector2D>({ topLeft + Line->start, topLeft + Line->end }),
			ESlateDrawEffect::None,
			FLinearColor::Gray,
			true,
			1.0f
		);
	}

	return RetLayerId;

}

void UW_CustomMap::InitializeWidget(float Tilesize)
{
	// UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(gridBorder->Slot);

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	/*if (!playerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("playerCharacter is nullptr"));
	}*/

	AUITestCharacter* uiTestPlayer = Cast<AUITestCharacter>(playerCharacter);
	if (!uiTestPlayer)
	{
		// UE_LOG(LogTemp, Warning, TEXT("uiTestPlayer is nullptr"));
	}
	if (uiTestPlayer)
	{
		itemComponent = uiTestPlayer->FindComponentByClass<UItemComponent>();
		// UE_LOG(LogTemp, Warning, TEXT("uiTestPlayer is not nullptr"));
	}


	canvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(gridBorder);

	if (canvasSlot)
	{

		/*float sizeX = itemComponent->columns * tileSize;
		float sizeY = itemComponent->rows * tileSize;*/
		float sizeX = itemComponent->columns * tileSize;
		float sizeY = itemComponent->rows * tileSize;

		canvasSlot->SetSize(FVector2D(sizeX, sizeY));

	}
	// CreateLineSegments();

	Refresh();
	
	itemComponent->OnInventoryChanged.AddDynamic(this, &UW_CustomMap::Refresh);
} 

void UW_CustomMap::Refresh()
{
	gridCanvasPanel->ClearChildren();

	TMap<UItemObject*, FTileStructureTemp> allItems = itemComponent->GetAllItems();

	TArray<UItemObject*> keys;
	allItems.GetKeys(keys);

	for (UItemObject* key : keys)
	{
		// CreateWidget()
		FTileStructureTemp* topLeftTile= allItems.Find(key);
		UItemObject* itemObejct = key;

		UW_ItemImg* newItemImg = CreateWidget<UW_ItemImg>(GetWorld(), ItemImgWidgetClass);

		if (newItemImg)
		{
			newItemImg->tileSize = tileSize;
			newItemImg->itemObject = itemObejct;


			if (newItemImg)
			{
				// 델리게이트 바인딩
				newItemImg->OnRemoved.AddDynamic(this, &UW_CustomMap::OnItemRemoved);
				
				UCanvasPanelSlot* tempCanvasSlot = Cast<UCanvasPanelSlot>(gridCanvasPanel->AddChild(newItemImg));

				if (tempCanvasSlot)
				{
					tempCanvasSlot->SetAutoSize(true);
					tempCanvasSlot->SetPosition(FVector2D(topLeftTile->X * tileSize, topLeftTile->Y * tileSize));
				}
			}
			
		}

	}
}

void UW_CustomMap::OnItemRemoved(class UItemObject* _ItemObject)
{
	itemComponent->RemoveItem(_ItemObject);
}

void UW_CustomMap::CreateLineSegments()
{
	lines.Empty();
	CreateVerticalLine();
	CreateHorizantalLine();

}

void UW_CustomMap::CreateVerticalLine()
{
	float X;

	for (int32 i = 0; i <= columns; ++i) {
		X = tileSize * i;

		ULineStructure* newLine = NewObject<ULineStructure>();
		newLine->start = FVector2D(X, 0.0f);
		newLine->end = FVector2D(X, tileSize * rows);

		lines.Add(newLine);
	}
}

void UW_CustomMap::CreateHorizantalLine()
{
	float Y;

	for (int32 i = 0; i <= rows; ++i) {
		Y = tileSize * i;

		ULineStructure* newLine = NewObject<ULineStructure>();
		newLine->start = FVector2D(0.0f, Y);
		newLine->end = FVector2D(tileSize * columns, Y);

		lines.Add(newLine);
	}
}


void UW_CustomMap::DrawGridLine()
{

	CreateLineSegments();

	if (IsInViewport())
	{
		Invalidate(EInvalidateWidget::LayoutAndVolatility);
	}

}

FVector2D UW_CustomMap::GetGridBorderTopLeft() const
{

	FGeometry geometry = gridBorder->GetCachedGeometry();
	FVector2D absolutePosition = geometry.GetAbsolutePosition();
	FVector2D localPosition = geometry.AbsoluteToLocal(absolutePosition);
	FVector2D borderSize = geometry.GetLocalSize();

	// 중앙 기준으로 절반 크기를 빼서 좌표 계산
	FVector2D topLeft = localPosition/* - (borderSize * 0.5f)*/;

	// FVector2D TopLeft = FVector2D(0.f, 0.f);

	/*UE_LOG(LogTemp, Warning, TEXT("Absolute position : (%f, %f)"), absolutePosition.X, absolutePosition.Y);
	UE_LOG(LogTemp, Warning, TEXT("Size : (%f, %f)"), borderSize.X, borderSize.Y);
	UE_LOG(LogTemp, Warning, TEXT("TopLeft position : (%f, %f)"), topLeft.X, topLeft.Y);*/


	/*FGeometry geometry = gridBorder->GetCachedGeometry();
	FVector2D localPosition = geometry.GetLocalSize() * -0.5f;

	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(gridBorder);
	if (CanvasSlot)
	{
		FVector2D AbsolutePosition = CanvasSlot->GetPosition();
		localPosition += AbsolutePosition;
	}

	FVector2D topLeft = localPosition;*/

	return topLeft;
}
