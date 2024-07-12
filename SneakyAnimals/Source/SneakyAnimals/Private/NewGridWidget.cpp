// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGridWidget.h"
#include "W_ItemImg.h"
#include <Components/Border.h>
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
#include "TileStructure.h"

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

	Invalidate(EInvalidateWidget::LayoutAndVolatility);

	// �� �ڵ� ��ġ ��� �ʿ�
	// itemImgWidget = CreateWidget<UW_ItemImg>(GetWorld(), itemImgWidgetClass);
	
	GetWorld()->GetTimerManager().SetTimer(SetGridSizeTimerHandle, this, &UNewGridWidget::DrawGridLine, 0.1f, false);

	GetWorld()->GetTimerManager().SetTimer(DrawGridLineTimerHandle, this, &UNewGridWidget::DrawGridLine, 0.2f, false);

}

void UNewGridWidget::OnItemRemoved(UItemObject* _ItemObject)
{
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
	FVector2D absolutePosition = geometry.GetAbsolutePosition();
	FVector2D localPosition = geometry.AbsoluteToLocal(absolutePosition);
	FVector2D borderSize = geometry.GetLocalSize();


	FVector2D topLeft = localPosition;

	return topLeft;
}

void UNewGridWidget::GridBorderSetSize(float _TileSize)
{
	canvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(gridBorder);
	
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (playerCharacter)
	{
		ATestPlayer* myPlayer = Cast<ATestPlayer>(playerCharacter);

		if (myPlayer)
		{
			itemComp = myPlayer->GetItemComponent();
			if (itemComp && IsValid(canvasSlot))
			{
				float sizeX = itemComp->columns * 100.f;
				float sizeY = itemComp->rows * 100.f;

				GetWorld()->GetTimerManager().SetTimer(SetGridSizeTimerHandle, this, &UNewGridWidget::DrawGridLine, 0.1f, false);

				// �̰� �ǹ̰� ���ݾ�.........
				canvasSlot->SetSize(FVector2D(sizeX, sizeY));

				// ���̾ƿ��� ĳ�̵� �����͸� ��ȿȭ�Ͽ� ��� �ݿ�
				Invalidate(EInvalidateWidget::LayoutAndVolatility);

				UE_LOG(LogTemp, Warning, TEXT("grid size: (%d, %d)"), itemComp->columns, itemComp->rows);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("itemComp is null || canvasPanel is null"));
			}
		}
	}

	Refresh();

	itemComp->OnInventoryChanged.AddDynamic(this, &UNewGridWidget::Refresh);

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

	TArray<UItemObject*> Keys;
	allItems.GenerateKeyArray(Keys);

	for (UItemObject* key : Keys)
	{
		// �׸��忡 ������ �߰��� ������ �տ������� ���� �����Ȱ� ���� ������ �� ���⵵
		FTileStructureTemp* topLeftTile = allItems.Find(key);
		UItemObject* itemObject = key;

		// item�� ��Ÿ���� ���� ����
		itemImgWidget = CreateWidget<UW_ItemImg>(GetWorld(), itemImgWidgetClass);

		if (itemImgWidget)
		{
			itemImgWidget->tileSize = tileSize;
			itemImgWidget->itemObject = itemObject;

			itemImgWidget->OnRemoved.AddDynamic(this, &UNewGridWidget::OnItemRemoved);

			// �� �� �ڿ� �� �̾ �ۼ��ؾ� �� �׷��� ���� ���̾��Ű ���鼭 Ȯ���غ� ����..? ���� �̻��ϴ� ������..???
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