// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataStructure.h"
#include "FL_General.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UFL_General : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	static bool Save_UserProfile(const FStructure_UserProfile& S_UserData);

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	static FUserProfileResult Get_UserProfile();
};
