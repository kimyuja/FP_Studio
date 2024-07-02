// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNEAKYANIMALS_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constants")
	int32 columns;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constants")
	int32 rows;

	UFUNCTION(BlueprintCallable)
	bool TryAddItem(class UItemObject* itemObject);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constants")
	TArray<class UItemObject*> Items;
	
};
