// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGridWidget.h"
#include "W_ItemImg.h"
#include <array>
#include <Components/CanvasPanel.h>
#include "Components/CanvasPanelSlot.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/UserWidget.h>
#include "Slate/WidgetTransform.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "TestPlayer.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h>
#include "ItemComponent.h"
#include "LineStructure.h"
#include <../../../../../../../Source/Runtime/Core/Public/Delegates/Delegate.h>
#include "Layout/Geometry.h"
#include "Rendering/DrawElements.h"
#include "ItemObject.h"
#include "Input/Reply.h"
#include "TileStructure.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Border.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/DragDropOperation.h>
#include "MyDragDropOperation.h"
#include <../../../../../../../Source/Runtime/SlateCore/Public/Input/DragAndDrop.h>

bool UNewGridWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;
	if (!gridBorder) return false;
	if (!gridCanvasPanel) return false;

	gridBorder->SetPadding(0.f);

	return true;
}

void UNewGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (gridBorder && gridCanvasPanel)
	{
		gridBorder->SetContent(gridCanvasPanel);
	}

	if (gridBorder)
	{
		gridBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("OnGridBorderMouseButtonDown"));
	}

	Invalidate(EInvalidateWidget::LayoutAndVolatility);

	// itemImgWidget = CreateWidget<UW_ItemImg>(GetWorld(), itemImgWidgetClass);

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (playerCharacter)
	{
		ATestPlayer* myPlayer = Cast<ATestPlayer>(playerCharacter);

		if (myPlayer)
		{
			itemComp = myPlayer->GetItemComponent();

			if (!itemComp)
			{
				UE_LOG(LogTemp, Error, TEXT("itemComp is null"));
				return;
			}
		}
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(SetGridSizeTimerHandle, [this]()
			{
				this->GridBorderSetSize(tileSize);
			}, 0.1f, false);

		GetWorld()->GetTimerManager().SetTimer(DrawGridLineTimerHandle, [this]()
			{
				this->DrawGridLine();
			}, 0.2f, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWorld() call failed"));
	}

	itemComp->OnInventoryChanged.AddDynamic(this, &UNewGridWidget::Refresh);
}

void UNewGridWidget::OnItemRemoved(UItemObject* _ItemObject)
{
	// itemObject = _ItemObject;
	itemComp->RemoveItem(_ItemObject);
}

void UNewGridWidget::DrawGridLine()
{
	CreateLineSegments();

	if (IsInViewport())
	{
		Invalidate(EInvalidateWidget::LayoutAndVolatility);
	}
}

FVector2D UNewGridWidget::GetGridBorderTopLeft() const
{
	FGeometry geometry = gridBorder->GetCachedGeometry();
	FVector2D tempPos = geometry.GetAbsolutePosition();

	UE_LOG(LogTemp, Warning, TEXT("Grid Abs Pos X : %f, Y : %f"), tempPos.X, tempPos.Y);

	FVector2D absolutePosition = geometry.GetAbsolutePosition();
	FVector2D localPosition = geometry.AbsoluteToLocal(absolutePosition);
	FVector2D borderSize = geometry.GetLocalSize();

	FVector2D topLeft = localPosition/* - (borderSize * 0.5f)*/;

	return topLeft;
}

FReply UNewGridWidget::OnGridBorderMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

UItemObject* UNewGridWidget::GetPayload(UDragDropOperation* _DragDropOperation) const
{
	if (false == IsValid(_DragDropOperation))
	{
		return nullptr;
	}
	else
	{
		return Cast<UItemObject>(_DragDropOperation->Payload);
	}
}

bool UNewGridWidget::IsRoomAvailableForPayload(UItemObject* _Payload) const
{
	if (IsValid(_Payload))
	{
		// 어라라 draggedItemTopLeft값이 지금 지정을 안해줬으니까 계속 0아닌가..?
		// 여기 이상한거 같은데
		FTileStructureTemp TopLeftTile;
		TopLeftTile.X = draggedItemTopLeft.X;
		TopLeftTile.Y = draggedItemTopLeft.Y;

		int32 topLeftIdx = itemComp->TileToIndex(TopLeftTile);

		// 그래서 여기가 계속 (0,0)으로만 찍히고 이상해지는거 맞는거 같어..
		return itemComp->IsRoomAvailable(_Payload, topLeftIdx);
	}
	return false;
}

void UNewGridWidget::GridBorderSetSize(float _TileSize)
{
	canvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(gridBorder);

	if (itemComp && IsValid(canvasSlot))
	{
		float sizeX = itemComp->columns * tileSize;
		float sizeY = itemComp->rows * tileSize;

		/*GetWorld()->GetTimerManager().SetTimer(SetGridSizeTimerHandle, this, &UNewGridWidget::DrawGridLine, 0.1f, false);*/

		canvasSlot->SetSize(FVector2D(sizeX, sizeY));

		// 레이아웃과 캐싱된 데이터를 무효화하여 즉시 반영
		Invalidate(EInvalidateWidget::LayoutAndVolatility);

		UE_LOG(LogTemp, Warning, TEXT("grid size: (%d, %d)"), itemComp->columns, itemComp->rows);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("itemComp is null || canvasPanel is null"));
	}

	Refresh();

	// itemComp->OnInventoryChanged.AddDynamic(this, &UNewGridWidget::Refresh);

}

void UNewGridWidget::Refresh()
{
	UE_LOG(LogTemp, Warning, TEXT("refresh func start"));

	if (gridCanvasPanel)
	{
		gridCanvasPanel->ClearChildren();
	}
	else if (!gridCanvasPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("gridcanvaspanel is null"));
	}

	TMap<UItemObject*, FTileStructureTemp> allItems = itemComp->GetAllItems();

	UE_LOG(LogTemp, Warning, TEXT("allitems Num : %d"), allItems.Num());

	TArray<UItemObject*> Keys;
	allItems.GenerateKeyArray(Keys);

	UCanvasPanel* rootCanvas = Cast<UCanvasPanel>(gridCanvasPanel);

	for (UItemObject* key : Keys)
	{
		FTileStructureTemp* topLeftTile = allItems.Find(key);
		UItemObject* itemObject = key;

		UW_ItemImg* newItemImgWidget = CreateWidget<UW_ItemImg>(GetWorld(), itemImgWidgetClass);
		UE_LOG(LogTemp, Warning, TEXT("widget!"));
		if (newItemImgWidget)
		{
			newItemImgWidget->tileSize = tileSize;
			// UE_LOG(LogTemp, Warning, TEXT("!!! tileSize : %f"), tileSize);

			//newItemImgWidget->thisItemObject = itemObject;
			newItemImgWidget->SetItemObject(itemObject);

			newItemImgWidget->OnRemoved.AddDynamic(this, &UNewGridWidget::OnItemRemoved);

			UCanvasPanelSlot* imgSlot = rootCanvas->AddChildToCanvas(newItemImgWidget);

			if (imgSlot)
			{
				// imgSlot->SetZOrder(99);
				imgSlot->SetSize(FVector2D(itemObject->dimensions.X * tileSize, itemObject->dimensions.Y * tileSize));
				// UE_LOG(LogTemp, Warning, TEXT("!!! itemObject !!!!!!! : %d %d"), itemObject->dimensions.X, itemObject->dimensions.Y);
				imgSlot->SetPosition(FVector2D(topLeftTile->X * tileSize, topLeftTile->Y * tileSize));
				// ESlateVisibility visibility = newItemImgWidget->GetVisibility();

			}

			UE_LOG(LogTemp, Warning, TEXT("position: (%f, %f)"), imgSlot->GetPosition().X, imgSlot->GetPosition().Y);
			UE_LOG(LogTemp, Warning, TEXT("size : (%f, %f)"), imgSlot->GetSize().X, imgSlot->GetSize().Y);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("fail to new item image widget create"));
		}
	}
}

void UNewGridWidget::CreateLineSegments()
{
	lines.Empty();
	CreateVerticalLine();
	CreateHorizantalLine();
}

void UNewGridWidget::CreateVerticalLine()
{
	float X;

	for (int32 i = 0; i <= itemComp->columns; ++i) {
		X = tileSize * i;

		ULineStructure* newLine = NewObject<ULineStructure>();
		newLine->start = FVector2D(X, 0.0f);
		newLine->end = FVector2D(X, tileSize * itemComp->rows);

		lines.Add(newLine);
	}
}

void UNewGridWidget::CreateHorizantalLine()
{
	float Y;

	for (int32 i = 0; i <= itemComp->rows; ++i) {
		Y = tileSize * i;

		ULineStructure* newLine = NewObject<ULineStructure>();
		newLine->start = FVector2D(0.0f, Y);
		newLine->end = FVector2D(tileSize * itemComp->columns, Y);

		lines.Add(newLine);
	}
}

int32 UNewGridWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 RetLayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	FVector2D topLeft = GetGridBorderTopLeft();
	// UE_LOG(LogTemp, Warning, TEXT("topLeft pos = (%f, %f)"), topLeft.X, topLeft.Y);

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

bool UNewGridWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	/*auto MousePosition = FSlateApplication::Get().GetCursorPos();

	UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop Abs Pos X : %f, Y : %f"), MousePosition.X, MousePosition.Y);

	FVector2D gridStartPos(2455.822266, 215.882034);

	FVector2D Dist = MousePosition - gridStartPos;*/

	if (1)
	{
		if (UMyDragDropOperation* ItemOperation = Cast<UMyDragDropOperation>(InOperation))
		{
			// auto tempPayLoad = GetPayload(ItemOperation);
			UItemObject* tempPayLoad = GetPayload(ItemOperation);

			if (IsRoomAvailableForPayload(tempPayLoad))
			// if (1)
			{
				
				// UItemObject* dropPayload = tempPayLoad;

				FTileStructureTemp TopLeftTile;
				TopLeftTile.X = draggedItemTopLeft.X;
				TopLeftTile.Y = draggedItemTopLeft.Y;

				int32 topLeftIdx = itemComp->TileToIndex(TopLeftTile);
				
				itemComp->AddItemAt(GetPayload(ItemOperation), topLeftIdx);

				//topLeftIdx = (int32)Dist.X / 160 * 4 + ((int32)Dist.Y / 160);

				/*if (dropPayload)
				{
					itemComp->AddItemAt(dropPayload, topLeftIdx);

				}*/
				return true;
			}
			return false;
		}
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WhY........WHy........."));
		return false;
	}

}

//bool UNewGridWidget::OnDragOver(FGeometry MyGeometry, FPointerEvent PointerEvent, UDragDropOperation* Operation)
//{
//	Super::OnDragOver(MyGeometry, PointerEvent, Operation);
//
//
//	return true;
//
//}
