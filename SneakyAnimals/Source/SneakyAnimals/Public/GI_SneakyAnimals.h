// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "DataStructure.h"
#include "GI_SneakyAnimals.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreateSessionSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreateSessionFailure);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFindSessionsCompleteCustom, const TArray<FMyBlueprintSessionResult>&, SessionResults);

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UGI_SneakyAnimals : public UGameInstance
{
	GENERATED_BODY()

public:
	UGI_SneakyAnimals();

	virtual void Init() override;

    UFUNCTION(BlueprintCallable, Category = "Network")
    void CreateSession(APlayerController* PlayerController, int32 MaxConnections, bool UseLAN, bool UseLobbies);

    //UFUNCTION(BlueprintCallable, Category = "Network")
    //void FindSessions(APlayerController* PlayerController, int32 MaxResults_SessionsToFind, bool bSearchForLAN);

    UPROPERTY(BlueprintAssignable, Category = "Network")
    FOnCreateSessionSuccess OnCreateSessionSuccess;

    UPROPERTY(BlueprintAssignable, Category = "Network")
    FOnCreateSessionFailure OnCreateSessionFailure;

    //UPROPERTY(BlueprintAssignable, Category = "Network")
    //FOnFindSessionsComplete OnFindSessionsCompleteCustom;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    FStructure_SessionInfo S_SessionInfo;

protected:
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    //void OnFindSessionsComplete(bool bWasSuccessful);

private:
    IOnlineSessionPtr SessionInterface;
    TSharedPtr<FOnlineSessionSettings> SessionSettings;
    TSharedPtr<FOnlineSessionSearch> SessionSearch;
    FName GetDynamicSessionName();
};
