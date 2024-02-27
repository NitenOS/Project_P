// Fill out your copyright notice in the Description page of Project Settings.


#include "testCharacter.h"

// Sets default values
AtestCharacter::AtestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("mainCamera"));
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	Camera->SetupAttachment(RootComponent);
	APlayerController* ShooterPlayerController = Cast<APlayerController>(Controller);
}

// Called when the game starts or when spawned
void AtestCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set the base speed for the character (600 is base speed)
	GetCharacterMovement()->MaxWalkSpeed = 600 * walkSpeed;
	
}

// Called every frame
void AtestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//It's just a thing for me to understand how to debug
	FString debugs = GetVelocity().ToString();
	FString debug = FString::SanitizeFloat(CameraInput.Y);
	GEngine->AddOnScreenDebugMessage(-1, 0.10f, FColor::Blue, debug);


	{ //Look up/down
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw = CameraInput.X;
		SetActorRotation(NewRotation); 
	}

	{ //Look left/right
		FRotator NewRotation = GetActorRotation();
		// Limit the rotation of the camera (not do a barrel roll)
		NewRotation.Pitch = FMath::Clamp(CameraInput.Y, -80.0f, 80.0f);
		SetActorRotation(NewRotation);
	}

}


void AtestCharacter::MoveForward(float value) {
	AddMovementInput(GetActorForwardVector(), value);
}

void AtestCharacter::MoveSide(float value) {
	AddMovementInput(GetActorRightVector(), value);
}

void AtestCharacter::MoveRunBegin() {
	//Code for run action
	GetCharacterMovement()->MaxWalkSpeed = 600 * walkSpeed * runSpeed;
}

void AtestCharacter::MoveRunEnd() {
	//Code for run action
	GetCharacterMovement()->MaxWalkSpeed = 600 * walkSpeed;
}

void AtestCharacter::LookUp(float value) {
	CameraInput.Y += value * cameraSpeed;
	// Limit the rotation of the camera (not do a barrel roll)
	CameraInput.Y = FMath::Clamp(CameraInput.Y, -80.0f, 80.0f);


}

void AtestCharacter::LookSide(float value) {
	CameraInput.X += value * cameraSpeed;

}

void AtestCharacter::BeginHide() {
	//Code for begin hide action
}

void AtestCharacter::EndHide() {
	//Code for exit hide action
}

// Called to bind functionality to input
void AtestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Move
	PlayerInputComponent->BindAxis("ForwardAxis", this, &AtestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("SideAxis", this, &AtestCharacter::MoveSide);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AtestCharacter::MoveRunBegin);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AtestCharacter::MoveRunEnd);


	// Look
	PlayerInputComponent->BindAxis("LockPitchAxis", this, &AtestCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookYawAxis", this, &AtestCharacter::LookSide);

	// Hide
	PlayerInputComponent->BindAction("Hide", IE_Pressed, this, &AtestCharacter::BeginHide);
	PlayerInputComponent->BindAction("Hide", IE_Released, this, &AtestCharacter::EndHide);
}

