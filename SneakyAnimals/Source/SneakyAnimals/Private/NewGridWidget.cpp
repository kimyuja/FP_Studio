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
#include "Components/WidgetSwitcher.h"
#include "Layout/Geometry.h"
#include "Rendering/DrawElements.h"
#include "ItemObject.h"
#include "Input/Reply.h"
#include "TileStructure.h"
#include "Gimmick.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Border.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/DragDropOperation.h>
#include "MyDragDropOperation.h"
#include <../../../../../../../Source/Runtime/SlateCore/Public/Input/DragAndDrop.h>
#include "MapCustomWidget.h"
#include <../../../../../../../Source/Runtime/Core/Public/Templates/SharedPointer.h>
#include <../../../../../../../Source/Runtime/Slate/Public/Framework/Application/SlateApplication.h>
#include "Blueprint/WidgetBlueprintLibrary.h"
#include <../../../../../../../Source/Runtime/SlateCore/Public/Styling/SlateBrush.h>
#include <../../../../../../../Source/Runtime/SlateCore/Public/Styling/SlateColor.h>
#include "WH_BookshelfGimmick.h"
#include "WH_BroomstickGimmick.h"
#include "WH_PotionGimmick.h"
#include "WH_WitchCauldronGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>

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

	//FindAllGimmick();

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
		myPlayer = Cast<ATestPlayer>(playerCharacter);

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

	// UE_LOG(LogTemp, Warning, TEXT("Grid Abs Pos X : %f, Y : %f"), tempPos.X, tempPos.Y);

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
		FTileStructureTemp TopLeftTile;
		TopLeftTile.X = draggedItemTopLeft.X;
		TopLeftTile.Y = draggedItemTopLeft.Y;

		int32 topLeftIdx = itemComp->TileToIndex(TopLeftTile);

		return itemComp->IsRoomAvailable(_Payload, topLeftIdx);
	}
	return false;
}

void UNewGridWidget::MousePositionInTile(FVector2D _MousePos)
{
	float mousePosX = (int32)_MousePos.X % (int32)tileSize;
	float mousePosY = (int32)_MousePos.Y % (int32)tileSize;
	float tileHalfSize = tileSize / 2.f;

	bRight = _MousePos.X > tileHalfSize;
	bDown = _MousePos.Y > tileHalfSize;
}

void UNewGridWidget::BindItemObjByBtn(TSubclassOf<AGimmick> _GimmickClass, int32 _ActiveType)
{
	if (myPlayer)
	{
		itemComp = myPlayer->GetItemComponent();
	}
	else
	{
		ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		if (playerCharacter)
		{
			myPlayer = Cast<ATestPlayer>(playerCharacter);

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
	}

	if (_GimmickClass)
	{
		if (_GimmickClass->IsChildOf(AWH_BookshelfGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is AWH_BookshelfGimmick"));

			for (TActorIterator<AWH_BookshelfGimmick> It(GetWorld()); It; ++It)
			{
				bookShelfActorArr.Add((*It));
				//UE_LOG(LogTemp, Warning, TEXT("AWH_BookshelfGimmick's Name : %s, PTR : %p"), *It->GetActorNameOrLabel(), *It);
				//UE_LOG(LogTemp, Warning, TEXT("bookShelfActor %d loc (%f, %f, %f)"), (*It)->newItemObject->itemActiveType, (*It)->GetActorLocation().X, (*It)->GetActorLocation().Y, (*It)->GetActorLocation().Z);
			}

			//UE_LOG(LogTemp, Warning, TEXT("bookShelfActorArr Size : %d"), bookShelfActorArr.Num());

			for (AWH_BookshelfGimmick* bookShelfActor : bookShelfActorArr)
			{
				// bookShelfActorArr[i]
				// UE_LOG(LogTemp, Warning, TEXT("arr start!!"));

				if (bookShelfActor)
				{

					bookShelfActor->Myactivetype = _ActiveType;

					itemObject = bookShelfActor->GetDefaultItemObject();

					// bookShelfActor->SetActorLocation(worldPosition);


					// UE_LOG(LogTemp, Warning, TEXT("bookShelfActor pos in world : (%f, %f, %f"), worldPosition.X, worldPosition.Y, worldPosition.Z);
					/*FVector newLocation = GridToWorld(bookShelfActor->GetGridX(), bookShelfActor->GetGridY());
					bookShelfActor->SetActorLocation(newLocation);*/

					// bookShelfActor->SetActorLocation(FVector(50130.f, -50100.f, -40.f));
					UE_LOG(LogTemp, Warning, TEXT("position changed"));


				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("bookShelfActor is null"));
				}
			}
		}
		else if (_GimmickClass->IsChildOf(AWH_BroomstickGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is AWH_BroomstickGimmick"));

			for (TActorIterator<AWH_BroomstickGimmick> It(GetWorld()); It; ++It)
			{
				broomStickActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("broomStickActorArr Num is %d"), broomStickActorArr.Num());

			for (AWH_BroomstickGimmick* broomStickActor : broomStickActorArr)
			{

				broomStickActor->Myactivetype = _ActiveType;

				itemObject = broomStickActor->GetDefaultItemObject();


			}
		}
		else if (_GimmickClass->IsChildOf(AWH_PotionGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is AWH_PotionGimmick"));

			for (TActorIterator<AWH_PotionGimmick> It(GetWorld()); It; ++It)
			{
				potionActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("potionActorArr Num is %d"), potionActorArr.Num());

			for (AWH_PotionGimmick* potionActor : potionActorArr)
			{
				potionActor->Myactivetype = _ActiveType;

				itemObject = potionActor->GetDefaultItemObject();


			}
		}
		else if (_GimmickClass->IsChildOf(AWH_WitchCauldronGimmick::StaticClass()))
		{
			for (TActorIterator<AWH_WitchCauldronGimmick> It(GetWorld()); It; ++It)
			{
				cauldronActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("cauldronActorArr Num is %d"), cauldronActorArr.Num());

			for (AWH_WitchCauldronGimmick* cauldronActor : cauldronActorArr)
			{
				// cauldronActor->newItemObject->itemActiveType = _ActiveType;
				cauldronActor->Myactivetype = _ActiveType;

				itemObject = cauldronActor->GetDefaultItemObject();


			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("!!! so sad"));
		}

		itemComp->TryAddItem(itemObject);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("!!! GimmickClass is empty"));
	}

}

AGimmick* UNewGridWidget::FindMatchingActor(UItemObject* _itemObject)
{
	for (TActorIterator<AGimmick> It(GetWorld()); It; ++It)
	{
		AGimmick* actor = *It;

		if (AWH_BookshelfGimmick* BookshelfActor = Cast<AWH_BookshelfGimmick>(actor))
		{
			if (BookshelfActor->newItemObject == _itemObject)
			{
				return BookshelfActor;
			}
		}
		else if (AWH_BroomstickGimmick* BroomstickActor = Cast<AWH_BroomstickGimmick>(actor))
		{
			if (BroomstickActor->newItemObject == _itemObject)
			{
				return BroomstickActor;
			}
		}
		else if (AWH_PotionGimmick* PotionActor = Cast<AWH_PotionGimmick>(actor))
		{
			if (PotionActor->newItemObject == _itemObject)
			{
				return PotionActor;
			}
		}
		else if (AWH_WitchCauldronGimmick* CauldronActor = Cast<AWH_WitchCauldronGimmick>(actor))
		{
			if (CauldronActor->newItemObject == _itemObject)
			{
				return CauldronActor;
			}
		}
	}

	return nullptr;
}
int32 UNewGridWidget::GetSwitcherIdx(AGimmick* _GimmickClass)
{
	if (_GimmickClass->IsA(AWH_BookshelfGimmick::StaticClass()))
	{
		return 0;
	}
	else if (_GimmickClass->IsA(AWH_BroomstickGimmick::StaticClass()))
	{
		return 1;
	}
	else if (_GimmickClass->IsA(AWH_PotionGimmick::StaticClass()))
	{
		return 2;
	}
	else if (_GimmickClass->IsA(AWH_WitchCauldronGimmick::StaticClass()))
	{
		return 3;
	}
	else
	{
		return -1;
	}
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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("gridcanvaspanel is null"));
		return;
	}

	TMap<UItemObject*, FTileStructureTemp> allItems = itemComp->GetAllItems();

	UE_LOG(LogTemp, Warning, TEXT("allitems Num : %d"), allItems.Num());

	TArray<UItemObject*> Keys;
	allItems.GenerateKeyArray(Keys);

	UCanvasPanel* rootCanvas = Cast<UCanvasPanel>(gridCanvasPanel);


	// 그리드의 좌표를 실제 게임 레벨 내 좌표로 변환
	auto GridToWorld = [&](int32 gridX, int32 gridY) -> FVector {
		float worldX = WHTopLeft.X + gridX * levelTileSize;
		float worldY = WHTopLeft.Y + gridY * levelTileSize;

		return FVector(worldX, worldY, WHTopLeft.Z);
	};


	for (UItemObject* key : Keys)
	{
		FTileStructureTemp* topLeftTile = allItems.Find(key);
		// UItemObject* itemObject = key;
		itemObject = key;

		UW_ItemImg* newItemImgWidget = CreateWidget<UW_ItemImg>(GetWorld(), itemImgWidgetClass);
		UE_LOG(LogTemp, Warning, TEXT("widget!"));
		if (newItemImgWidget)
		{
			newItemImgWidget->tileSize = tileSize;
			// UE_LOG(LogTemp, Warning, TEXT("!!! tileSize : %f"), tileSize);

			//newItemImgWidget->thisItemObject = itemObject;
			newItemImgWidget->SetItemObject(itemObject);

			AGimmick* gm = FindMatchingActor(itemObject);
			int32 switcherIdx = GetSwitcherIdx(gm);

			if (switcherIdx != -1 && newItemImgWidget->itemImgSwitcher)
			{
				newItemImgWidget->itemImgSwitcher->SetActiveWidgetIndex(switcherIdx);
			}

			newItemImgWidget->OnRemoved.AddDynamic(this, &UNewGridWidget::OnItemRemoved);

			UCanvasPanelSlot* imgSlot = rootCanvas->AddChildToCanvas(newItemImgWidget);

			if (imgSlot)
			{
				// imgSlot->SetZOrder(99);
				imgSlot->SetSize(FVector2D(itemObject->dimensions.X * tileSize, itemObject->dimensions.Y * tileSize));
				// UE_LOG(LogTemp, Warning, TEXT("!!! itemObject !!! : %d %d"), itemObject->dimensions.X, itemObject->dimensions.Y);
				imgSlot->SetPosition(FVector2D(topLeftTile->X * tileSize, topLeftTile->Y * tileSize));
				// ESlateVisibility visibility = newItemImgWidget->GetVisibility();

				// 월드좌표의 topLeft 기준 위치 맞추기
				worldPosition = GridToWorld(topLeftTile->X, topLeftTile->Y);
				UE_LOG(LogTemp, Warning, TEXT("worldPosition is : (%f, %f, %f) "), worldPosition.X, worldPosition.Y, worldPosition.Z);

				// 아이템 dimenstion 따라 위치 맞추기
				FVector dimensionOffset = FVector(itemObject->dimensions.X * levelTileSize * 0.5f, itemObject->dimensions.Y * levelTileSize * 0.5f, 0.0f);

				// 최종 위치해야하는 pos 
				FVector adjustedWorldPosition = worldPosition + dimensionOffset;

				AGimmick* gimmickActor = FindMatchingActor(itemObject);

				if (gimmickActor)
				{
					// 현재 레벨에 배치된 상태라면 X, Y 위치만 변경
					// 레벨에 배치되지 않은 상태라면 Z 값도 같이 변경
					FVector currentLoc = gimmickActor->GetActorLocation();

					if (currentLoc.Z > 0)
					{
						adjustedWorldPosition.Z = currentLoc.Z;
					}
					else
					{
						adjustedWorldPosition.Z = currentLoc.Z + 1000.f;
					}
					myPlayer->ServerRPC_SetGActorLoc(gimmickActor, adjustedWorldPosition, gimmickActor->activeType);
					//gimmickActor->SetActorLocation(adjustedWorldPosition);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("I CANT FIND MACHINGED ACTOR"));
				}

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
			FLinearColor(0.5f, 0.5f, 0.5f, 0.5f),
			true,
			1.0f
		);
	}

	if (FSlateApplication::Get().IsDragDropping() && bDrawDropLoc)
	{
		//GetPayload(FSlateApplication::Get().GetDragDroppingContent());
		UItemObject* MyPayload = GetPayload(UWidgetBlueprintLibrary::GetDragDroppingContent());

		if (MyPayload)
		{
			FLinearColor boxColor = IsRoomAvailableForPayload(MyPayload) ? FLinearColor(0.0f, 1.0f, 0.0f, 0.25f) : FLinearColor(1.0f, 0.0f, 0.0f, 0.25f);

			FVector2D boxPos = (FVector2D)draggedItemTopLeft * tileSize;
			FVector2D boxSize;
			boxSize.X = MyPayload->dimensions.X * tileSize;
			boxSize.Y = MyPayload->dimensions.Y * tileSize;

			FPaintGeometry PaintGeometry = AllottedGeometry.ToPaintGeometry(FVector2f(boxSize), FSlateLayoutTransform(boxPos));

			/*boxBrush.TintColor = FSlateColor(FLinearColor::White);
			boxBrush.ImageSize = FVector2f(100.0f, 100.0f);
		*/
			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId,
				PaintGeometry,
				&boxBrush,
				ESlateDrawEffect::None,
				boxColor
			);
		}
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


	if (UMyDragDropOperation* ItemOperation = Cast<UMyDragDropOperation>(InOperation))
	{
		// auto tempPayLoad = GetPayload(ItemOperation);
		UItemObject* tempPayLoad = GetPayload(ItemOperation);

		if (IsRoomAvailableForPayload(tempPayLoad))
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
		else
		{
			// 놓으려고 하는 곳에 사용할 수 있는 공간이 없다면
			// 인벤토리 내에 들어있도록은 해야하기 때문에
			// 어디에 추가할지 신경쓰지 않고 사용 가능한 가장 좋은 슬롯에 추가되도록 하고싶다
			if (!itemComp->TryAddItem(tempPayLoad))
			{
				return true;
			}

		}
		return false;
	}
	return false;


}

bool UNewGridWidget::NativeOnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent, UDragDropOperation* Operation)
{
	Super::NativeOnDragOver(MyGeometry, DragDropEvent, Operation);

	// 마우스에 대한 정보인 포인터 이벤트를 얻어온다 (로컬 절대값)
	FVector2D MousePosition = MyGeometry.AbsoluteToLocal(DragDropEvent.GetScreenSpacePosition());

	MousePositionInTile(MousePosition);

	UItemObject* iobj = Cast<UItemObject>(GetPayload(Operation));

	FVector2D ClampedPosition;
	/*ClampedPosition.X = FMath::Clamp(MousePosition.X + (bRight ? iobj->dimensions.X : 0), 0.0f, tileSize);
	ClampedPosition.Y = FMath::Clamp(MousePosition.Y + (bDown ? iobj->dimensions.Y : 0), 0.0f, tileSize);*/

	ClampedPosition.X = FMath::Clamp(MousePosition.X, 0.0f, (bRight ? iobj->dimensions.X - 1 : iobj->dimensions.X));
	ClampedPosition.Y = FMath::Clamp(MousePosition.Y, 0.0f, (bDown ? iobj->dimensions.Y - 1 : iobj->dimensions.Y));

	FIntPoint intPosition;
	intPosition.X = (int32)ClampedPosition.X;
	intPosition.Y = (int32)ClampedPosition.Y;

	float tileX = MousePosition.X / tileSize;
	float tileY = MousePosition.Y / tileSize;

	// 나눈 결과를 정수로 변환 (정수 부분만 남기기)
	int32 truncatedX = (int32)(tileX);
	int32 truncatedY = (int32)(tileY);

	draggedItemTopLeft.X = truncatedX - intPosition.X / 2;
	draggedItemTopLeft.Y = truncatedY - intPosition.Y / 2;

	return true;
}

void UNewGridWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	bDrawDropLoc = true;
}

void UNewGridWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	bDrawDropLoc = false;
}

FReply UNewGridWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// R키가 눌리면 회전시키고 싶다
	FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::R)
	{
		UE_LOG(LogTemp, Warning, TEXT("R key was pressed"));

		// 드래그 앤 드롭 콘텐츠 가져오기
		UDragDropOperation* dragDroppingContent = UWidgetBlueprintLibrary::GetDragDroppingContent();
		
		UMyDragDropOperation* dragDropOperation = Cast<UMyDragDropOperation>(dragDroppingContent);


		if (dragDropOperation)
		{
			UItemObject* payLoadTemp = GetPayload(dragDropOperation);

			if (IsValid(payLoadTemp))
			{
				itemObject->RotateGA(payLoadTemp);

				FIntPoint NewDimensions = payLoadTemp->GetDimensions();
				payLoadTemp->SetDimensions(NewDimensions);

				UW_ItemImg* imgWidgetTemp = Cast<UW_ItemImg>(dragDropOperation->DefaultDragVisual);

				if (IsValid(imgWidgetTemp))
				{
					// imgWidgetTemp->Refresh();
					Refresh();

					return FReply::Handled();
				}
			}
		}
	}

	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

