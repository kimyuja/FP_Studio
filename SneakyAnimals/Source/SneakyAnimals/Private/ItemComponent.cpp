// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"
#include "ItemObject.h"
#include "TileStructure.h"
// #include <../../../../../../../Source/Runtime/CoreUObject/Public/UObject/NoExportTypes.h>

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	columns = 10;
	rows = 10;
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	int32 arraySize = columns * rows;

	// items �迭 resize
	items.SetNum(arraySize);

}


// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (isDirty)
	{
		isDirty = false;

		OnInventoryChanged.Broadcast();

	}
}

// TryAddItem�� ��ư�� ������ �������� ��ư�� itemObject�� �޾� ȣ�������� ��
bool UItemComponent::TryAddItem(UItemObject* _ItemObject)
{
	UE_LOG(LogTemp, Warning, TEXT("Start TryAddItem Func"));

	// �׸��忡 �߰��Ϸ��� �������� �ް� �� �������� ��ȿ���� Ȯ��
	if (IsValid(_ItemObject))
	{
		// leftTop ���� ������ rightBottom ���� �׸����� �� Ÿ�Ͽ� ���� �ش� Ÿ�� �ֺ��� �߰��� �������� ��ġ�� �� �ִ� ��ŭ�� ������ �ִ��� Ȯ��  
		for (int32 idx = 0; idx < items.Num(); idx++)
		{
			bool bRoomAvailable = IsRoomAvailable(_ItemObject, idx);
			UE_LOG(LogTemp, Warning, TEXT("idx: %d"), idx);

			// ������ �ִٸ� �ε����� �׸� �߰��ϰ� true ��ȯ
			if (bRoomAvailable)
			{
				AddItemAt(_ItemObject, idx);
				return true;
			}
		}
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tryed Item is not valid!"));
	}
	return false;
}

bool UItemComponent::IsRoomAvailable(UItemObject* __ItemObject, int32 __TopLeftIndex)
{
	// itemObject�� ���� ������ �����ϴ��� ���� Ȯ��
	FIntPoint itemDimension = __ItemObject->GetDimensions(__ItemObject);
	FTileStructureTemp topLeftTile = IndexToTile(__TopLeftIndex);

	int32 iLastIdx = topLeftTile.X + itemDimension.X;
	int32 jLastIdx = topLeftTile.Y + itemDimension.Y;

	// bool bFind = false;
	UE_LOG(LogTemp, Warning, TEXT("XXYY : %d, %d, %d, %d"), topLeftTile.X, itemDimension.X, topLeftTile.Y, itemDimension.Y);

	if (iLastIdx > columns || jLastIdx > rows)
	{
		return false;
	}

	/*UE_LOG(LogTemp, Warning, TEXT("topLeftTile: %d"), topLeftTile.X);
	UE_LOG(LogTemp, Warning, TEXT("iLastIdx: %d"), iLastIdx);*/


	// item�� leftTop�� �ε����� �߰��� ��� �����ϰ� �� �κ��丮 ������ Ÿ�� �ݺ� Ž��
	for (int32 i = topLeftTile.X; i < iLastIdx; i++)
	{
		for (int32 j = topLeftTile.Y; j < jLastIdx; j++)
		{

			FTileStructureTemp tile;
			tile.X = i;
			tile.Y = j;

			// Ÿ���� ��ȿ���� Ȯ��
			// return ���� false�� ��� : ��� ������ ���� ����
			// return ���� true�� ���(��ȿ�� ���) : ��� ������ ���� ���� => �ش� Ÿ�� ��ȯ
			if (!CheckEmptySlot(tile))
			{
				return false;
			}

		}
	}

	return true;
}

bool UItemComponent::CheckEmptySlot(FTileStructureTemp tile)
{
	bool bCheck = IsTileValid(tile);
	if (bCheck == false)
	{
		return false;
	}
	else
	{

		if (GetItemAtIndex(TileToIndex(tile)))
		{
			if (IsValid(outItemTemp))
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}
}

FTileStructureTemp UItemComponent::IndexToTile(int32 Index) const
{
	// 0 ���� n���� x�� y ��ǥ�� ��ȯ
	return FTileStructureTemp(Index % columns, Index / columns);
}

bool UItemComponent::IsTileValid(FTileStructureTemp Tile) const
{
	// Ÿ���� ��ȿ���� ���θ� �˷��ִ� ��ũ��

	bool condition1 = (Tile.X >= 0);
	bool condition2 = (Tile.Y >= 0);
	bool condition3 = (Tile.X < columns);
	bool condition4 = (Tile.Y < rows);

	bool allCondition = condition1 && condition2 && condition3 && condition4;

	return allCondition;
}

int32 UItemComponent::TileToIndex(FTileStructureTemp Tile) const
{
	return Tile.X + (Tile.Y * columns);
}

bool UItemComponent::GetItemAtIndex(int32 Index)
{
	bool bValid = items.IsValidIndex(Index);

	if (bValid)
	{
		outItemTemp = items[Index];
		return true;
	}
	else
	{
		outItemTemp = nullptr;
		return false;
	}


}

void UItemComponent::AddItemAt(UItemObject* _ItemObject_, int32 _TopLeftIndex_)
{
	FIntPoint itemDimension = _ItemObject_->GetDimensions(_ItemObject_);
	UE_LOG(LogTemp, Warning, TEXT("!!! ITEMCOMPONENT Dimension XY is %d %d"), itemDimension.X, itemDimension.Y)
	FTileStructureTemp topLeftTile = IndexToTile(_TopLeftIndex_);
	UE_LOG(LogTemp, Warning, TEXT("!!! topLeft tile X is %d"), topLeftTile.X);

	int32 iLastIdx = topLeftTile.X + itemDimension.X;
	int32 jLastIdx = topLeftTile.Y + itemDimension.Y;
	UE_LOG(LogTemp, Warning, TEXT("!!! iLastIdx is %d / jLastIdx is %d"), iLastIdx, jLastIdx);


	for (int32 i = topLeftTile.X; i < iLastIdx; i++)
	{
		for (int32 j = topLeftTile.Y; j < jLastIdx; j++)
		{
			FTileStructureTemp tile;
			tile.X = i;
			tile.Y = j;

			int32 itemIdx = TileToIndex(tile);

			// setArrayElem���� Items�� targetArray�� �ް� itemIdx�� Index�� �ް� ItmeObject�� Item���� ����
			items[itemIdx] = _ItemObject_;
		}
	}
	isDirty = true;
}

TMap<UItemObject*, FTileStructureTemp> UItemComponent::GetAllItems()
{
	TMap<UItemObject*, FTileStructureTemp> AllItems;

	// �迭�� ��� �׸� �Ǵ� ��� Ÿ���� �ݺ��ϰ� ������ �׸��� �ִ��� Ȯ��
	for (int32 idx = 0; idx < items.Num(); idx++)
	{
		UItemObject* currentItemObject = items[idx];

		if (IsValid(currentItemObject))
		{
			FTileStructureTemp currentTile = IndexToTile(idx);

			// �ʿ� �׸� �߰� ���� Ȯ��
			if (!AllItems.Contains(currentItemObject))
			{
				AllItems.Add(currentItemObject, currentTile);
			}
		}
	}

	return AllItems;
}

void UItemComponent::RemoveItem(UItemObject* _ItemObject)
{
	if(IsValid(_ItemObject))
	{
		// items�� item�� ����� item�� ��ġ�ϴ��� Ȯ���ϰ� ��ġ�Ѵٸ�
		// �⺻������ �ƹ��͵� �������� �ʰų� ��ȿȭ�ϱ⸦ ���Ѵ�
		for (int32 i = 0; i < items.Num(); i++)
		{
			if (items[i] == _ItemObject)
			{
				items[i] = nullptr;
			}
		}

		isDirty = true;
	}
}

void UItemComponent::ChangeInventory()
{
	isDirty = true;
}

bool UItemComponent::CheckIsValid(bool b)
{
	if (b == true) return true;
	else return false;
}
