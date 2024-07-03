// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStructure.h"
#include "TileStructure.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FTileStructureTemp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

	FTileStructureTemp() : X(0), Y(0) {}	// �⺻ ������
	FTileStructureTemp(int32 inX, int32 inY) : X(inX), Y(inY) {}	// �����ε�� ������

	bool operator==(const FTileStructureTemp& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}
};

// GetTypeHash �Լ��� �����Ͽ� FTileStructureTemp�� �ؽ� �����ϵ��� �Ѵ�
FORCEINLINE uint32 GetTypeHash(const FTileStructureTemp& Tile)
{
	return HashCombine(GetTypeHash(Tile.X), GetTypeHash(Tile.Y));
}

UCLASS()
class SNEAKYANIMALS_API UTileStructure : public UDataStructure
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

};
