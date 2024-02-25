// Fill out your copyright notice in the Description page of Project Settings.


#include "testCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AtestCharacter::AtestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AtestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Set the base speed for the character (600 is base speed)
	GetCharacterMovement()->MaxWalkSpeed = 600 * speed;
	
}

// Called every frame
void AtestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AtestCharacter::MoveForward(float value) {
	
	//It's just a thing for me to understand how to debug
	FString debugs = GetVelocity().ToString();
	FString debug = FString::SanitizeFloat(GetCharacterMovement()->MaxWalkSpeed);
	GEngine->AddOnScreenDebugMessage(-1, 0.10f, FColor::Blue, debugs);
	
	AddMovementInput(GetActorForwardVector(), value);
}

void AtestCharacter::MoveSide(float value) {
	AddMovementInput(GetActorRightVector(), value);
}

void AtestCharacter::LookUp(float value) {
	AddControllerPitchInput(value);
}

void AtestCharacter::LookSide(float value) {
	AddControllerYawInput(value);

}

// Called to bind functionality to input
void AtestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Move
	PlayerInputComponent->BindAxis("ForwardAxis", this, &AtestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("SideAxis", this, &AtestCharacter::MoveSide);

	// Look
	PlayerInputComponent->BindAxis("LockUpAxis", this, &AtestCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookSideAxis", this, &AtestCharacter::LookSide);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AtestCharacter::Jump);
}

