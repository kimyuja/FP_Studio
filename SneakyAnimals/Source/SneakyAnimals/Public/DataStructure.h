// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "DataStructure.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UDataStructure : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT(Atomic, BlueprintType)
struct FStructure_Available_Avatars : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Avatar;
};

USTRUCT(Atomic, BlueprintType)
struct FStructure_UserProfile : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Username;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* User_Avatar;
};

USTRUCT(Atomic, BlueprintType)
struct FUserProfileResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FStructure_UserProfile S_UserProfile;

    UPROPERTY(BlueprintReadWrite)
    bool success;
};

USTRUCT(Atomic, BlueprintType)
struct FStructure_Available_Characters : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* ItemImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USkeletalMesh* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;
};

USTRUCT(BlueprintType)
struct FStructure_Player_Appearance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FStructure_Available_Characters Character;
	
	//UPROPERTY(BlueprintReadOnly)
	//FStructure_Available_Accessories_Hat Hat_Slot;
};

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString roomName;
	UPROPERTY(BlueprintReadOnly)
	FString hostName;
	UPROPERTY(BlueprintReadOnly)
	FString userName;
	UPROPERTY(BlueprintReadOnly)
	int32 maxPlayerCount;
	UPROPERTY(BlueprintReadOnly)
	int32 currentPlayerCount;
	UPROPERTY(BlueprintReadOnly)
	int32 pingMs;

	int32 index;

	FORCEINLINE void Set(int32 _index, const FOnlineSessionSearchResult& item) {
		index = _index;
		//item.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
		//item.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);
		// 방장의 이름
		userName = item.Session.OwningUserName;
		// 최대 플레이어 수
		maxPlayerCount = item.Session.SessionSettings.NumPublicConnections;
		// 현재 방에 들어온 플레이어 수
		currentPlayerCount = maxPlayerCount - item.Session.NumOpenPublicConnections;
		pingMs = item.PingInMs;
	}

	FORCEINLINE FString ToString() {
		return FString::Printf(TEXT("%s, %s, %s, (%d/%d), %dms"), *roomName, *hostName, *userName, currentPlayerCount, maxPlayerCount, pingMs);
	}
};