// Fill out your copyright notice in the Description page of Project Settings.


#include "UITestCharacter.h"
#include "Engine.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include "Gimmick.h"
#include "WH_BookshelfGimmick.h"
#include "MapCustomWidget.h"

// Sets default values
AUITestCharacter::AUITestCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->SetRelativeLocation(FVector(0, 40, 60));
	cameraBoom->TargetArmLength = 300.0f;
	cameraBoom->bUsePawnControlRotation = true;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	camera->bUsePawnControlRotation = false;

	itemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("itemComponent"));

}

// Called when the game starts or when spawned
void AUITestCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(imc_testmove, 0);
		}
	}

}

// Called every frame
void AUITestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUITestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedinput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedinput) {
		// 이동
		enhancedinput->BindAction(ia_move, ETriggerEvent::Triggered, this, &AUITestCharacter::Move);
		// 점프
		enhancedinput->BindAction(ia_jump, ETriggerEvent::Started, this, &AUITestCharacter::PlayerJump);
		enhancedinput->BindAction(ia_jump, ETriggerEvent::Completed, this, &AUITestCharacter::PlayerJumpEnd);
		// 시야회전
		enhancedinput->BindAction(ia_look, ETriggerEvent::Triggered, this, &AUITestCharacter::Look);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail"));
	}

}

void AUITestCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUITestCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X / 2.0f);
		AddControllerPitchInput(-LookAxisVector.Y / 2.0f);
	}
}

void AUITestCharacter::PlayerJump(const FInputActionValue& Value)
{
	Jump();
}

void AUITestCharacter::PlayerJumpEnd(const FInputActionValue& Value)
{
	StopJumping();
}

//void AUITestCharacter::CreateMapCustomWidget()
//{
//	if (customMapWidgetClass != nullptr)
//	{
//		customMapWidget = CreateWidget<UMapCustomWidget>(GetWorld(), customMapWidgetClass);
//		if (customMapWidget != nullptr)
//		{
//			customMapWidget->AddToViewport();
//
//			customMapWidget->InitializeCustomMap(customMapWidgetClass, 160.f);
//		}
//	}
//}


