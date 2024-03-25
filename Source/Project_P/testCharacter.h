// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h" 
#include "Components/AudioComponent.h" 
#include "PhysicsEngine/PhysicsHandleComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h" 
#include "Kismet/GameplayStatics.h"
#include "Delegates/Delegate.h"
#include "adoChild.h"
#include "vaiselleTask.h"
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
	/** Multiply the walk speed (base : 600.0f) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPMovement)
		float walkSpeed = 600;
	/** Multiply the walk speed when press shift (base : walkSpeed) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPMovement)
		float runSpeed = 2;
	/** Speed of the mouse (base : 5.0f) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPMovement)
		float cameraSpeed = 5;



	/** base BPM (jauge) on start (max 100) (base : 1.0f) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CPPJauge)
		float stressBPM = 1.0f;
	/** max distance from ado to start up BPM (jauge) (base : 500 (int)) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPJauge)
		int maxDistanceAdo = 500;
	/** base YeetForce (addForce) on start (max 800) (base : 500.0f) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPJauge)
		float YeetForce = 500.0f;
		float saveYeetForce = 0.0f; 
		float saveFieldOfView = 0.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPSound)
		UAudioComponent* WalkSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPSound)
		UAudioComponent* HeartSFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPSound)
		TArray<USoundBase*> footsteps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPSound)
		TArray<USoundBase*> heartBeat;





	// footstep var
	int numFootstep = 0;
	float footstepcount = 0.0f;
	float maxFootstepCount = 0.8f;

	// heartbeat var
	int numHeartbeat = 0;
	float heartbeatCount = 0.0f;
	float actualHeartBeatCount = 1.5f;
	float minHeartbeatCount = 0.8f;
	float maxHeartbeatCount = 1.5f;

	// valk var
	bool isWalkingF = false;
	bool isWalkingD = false;
	bool isRuning = false;
	float countRuning = 0.0f;
	const float maxCountRuning = 0.5f;

	UWorld* gameWorld;
	AActor* adoclass;
	AadoChild* ado;
	float distance;
	bool isGrabed = false;

	bool isShoked = false;
	float countShoked = 0.0f;
	float maxCountShoked = 2.0f;

	bool isHide = false;
	float countHide = 0.0f;
	const float maxCountHide = 2.0f;

	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere)
	UPhysicsHandleComponent* PhyHandle;

	//UCapsuleComponent* Capsule;

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
	void HideEnd();

	// Grab function
	void GrabBegin();
	void GrabEnd();

	// Shit code
	void ResetBPM();
	void ChangeForm();




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//UPROPERTY(BlueprintAssignable)
	UFUNCTION(BlueprintCallable)
	AActor* Grabing();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPSound)
	bool humainForm = true;

	UFUNCTION(BlueprintCallable)
	void Shoked(FRotator childRotation);


	UFUNCTION(BlueprintCallable)
	/**Check "add" for add the stressBPM
	* uncheck "add" for subtracte stressBPM
	*/
	void changeBPM(bool add, float number = 10);

	UFUNCTION(BlueprintCallable)
	bool getIsHide();
	UFUNCTION(BlueprintCallable)
	void setIsHide(bool hide);

	UFUNCTION(BlueprintCallable)
	void Yeet();

	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName = "Hide component")
	AActor* hideChar(AActor* choseOne);
};
