// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "testCharacter.generated.h"

UCLASS()
class PROJECT_P_API AtestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AtestCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Simplifier Speed character
	/** Multiply the walk speed (base : 600) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPMovement)
		float walkSpeed = 1;
	/** Multiply the walk speed when press shift (base : 600xwalkSpeed) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPMovement)
		float runSpeed = 2;
	/** Speed of the mouse */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPMovement)
		float cameraSpeed = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPMovement)
		USceneComponent* grabObjectPoint;

	UWorld* gameWorld;

	// Camera
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	FVector2D CameraInput;

	// Movement functions
	void MoveForward(float value);
	void MoveSide(float value);
	void MoveRunBegin();
	void MoveRunEnd();

	// Look functions
	void LookUp(float value);
	void LookSide(float value);

	// Hide function
	void HideBegin();
	void HideEnd();

	// Grab function
	void GrabBegin();
	void GrabEnd();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
