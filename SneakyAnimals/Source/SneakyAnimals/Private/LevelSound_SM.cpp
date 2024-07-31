// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSound_SM.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelSound_SM::ALevelSound_SM()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALevelSound_SM::BeginPlay()
{
	Super::BeginPlay();

	if (AudioComponent && AudioComponent->Sound)
	{
		AudioComponent->Play();
	}
	
}

// Called every frame
void ALevelSound_SM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelSound_SM::FadeOutSound(float FadeOutDuration)
{
	if (AudioComponent)
	{
		AudioComponent->FadeOut(FadeOutDuration, 0.0f);

		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				AudioComponent->Stop();
		});

	}
}

