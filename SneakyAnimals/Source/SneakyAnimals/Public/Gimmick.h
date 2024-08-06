// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gimmick.generated.h"

UCLASS()
class SNEAKYANIMALS_API AGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGimmick();

	UPROPERTY(EditAnywhere)
	bool bIsFinished = false;

	UPROPERTY(EditAnywhere)
	bool bCanActive = false;

	UPROPERTY(EditAnywhere)
	int32 activeType = 0;

	UPROPERTY(EditAnywhere)
	class ASAGameStateBase* gameState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float timerCheck;

	UPROPERTY(EditAnywhere)
	TArray<class USoundBase*> sounds;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated)
	int32 _key;

	UPROPERTY(Replicated)
	int32 soundNum;

	UPROPERTY(Replicated)
	bool bIsPlayed = false;

	UFUNCTION()
	int32 OnMyActive(AActor* ActivePlayer);

	UFUNCTION()
	void SetActiveType(int32 aType);
	
	UFUNCTION(BlueprintCallable, Category = "Items")
	virtual UItemObject* GetDefaultItemObject();

	UFUNCTION(BlueprintCallable, Category = "Items")
	void InitializeItemObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UItemObject* itemObject;

	UPROPERTY()
	class UPlayerDieLogWidget* playerDieLogWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> playerDieLogWidgetClass;

	UFUNCTION()
	void SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
