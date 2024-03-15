// Fill out your copyright notice in the Description page of Project Settings.


#include "testCharacter.h"

// Sets default values
AtestCharacter::AtestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Camera setup
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("mainCamera"));
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	Camera->SetRelativeRotation(FQuat(0.0f));
	Camera->SetupAttachment(RootComponent);

	//Player Controller setup
	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	//Physics Handle setup
	PhyHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("phyHandle"));
	
	//walkSFX setup
	WalkSFX = CreateDefaultSubobject<UAudioComponent>(TEXT("walkSFX"));
	WalkSFX->SetRelativeLocation(FVector(0.0f, 0.0f, -70.0f));
	WalkSFX->SetupAttachment(RootComponent);

	HeartSFX = CreateDefaultSubobject<UAudioComponent>(TEXT("heartSFX"));
	HeartSFX->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));
	HeartSFX->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AtestCharacter::BeginPlay()
{
	Super::BeginPlay();
	gameWorld = GetWorld();

	// Set the base speed for the character (600 is base speed)
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	adoclass = UGameplayStatics::GetActorOfClass(gameWorld, AadoChild::StaticClass());
	ado = Cast<AadoChild>(adoclass);

	// Check value
	if (stressBPM > 100.0f) { stressBPM = 100.0f; }
	if (YeetForce > 800.0f) { YeetForce = 800.0f; }

	//Camera->SetRelativeRotation(FQuat(0.0f));
	this->SetActorRotation(Camera->GetRelativeRotation());
	
}

// Called every frame
void AtestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Debug message
	{
		FString rotPlayer = this->CameraInput.ToString();
		FString rotCam = Camera->GetRelativeRotation().ToString();
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, rotPlayer);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, rotCam);
	}

	//Look left/right
	{ 
		FRotator NewRotationX = GetActorRotation();
		NewRotationX.Yaw = CameraInput.X;
		SetActorRotation(NewRotationX);
	}
	
	//Look up/down
	{ 
		FRotator NewRotationY = Camera->GetRelativeRotation();
		// Limit the rotation of the camera (not do a barrel roll)
		NewRotationY.Pitch = FMath::Clamp(CameraInput.Y, -80.0f, 80.0f);
		Camera->SetRelativeRotation(NewRotationY);
	}

	// Stress jauge
	{
		if (ado != nullptr) {
			distance = GetDistanceTo(ado);
			//GEngine->AddOnScreenDebugMessage(-1, 0.10f, FColor::Blue, FString::SanitizeFloat(distance));
		
			if (distance <= maxDistanceAdo && stressBPM <= 100) {
				stressBPM += 0.5;
			}
			else if (distance >= maxDistanceAdo && stressBPM >=0) {
				stressBPM -= 0.5;
			}
		
		}
	}

	// Camera FX
	{
		Camera->PostProcessSettings.VignetteIntensity = 3 * stressBPM * 0.01;
		Camera->PostProcessSettings.FilmGrainIntensity = 10 * stressBPM * 0.01;
	}

	// Grab system
	{
		if (isGrabed) {
			PhyHandle->SetTargetLocation(Camera->GetForwardVector() * 200 + Camera->GetComponentLocation());
		}
	}

	// Sound FX
	{
		// Footstep 
		{
			WalkSFX->Sound = footsteps[numFootstep];

			if (isWalkingF || isWalkingD) {
				footstepcount += DeltaTime;
				if (isRuning) { footstepcount += DeltaTime; }

				if (footstepcount >= maxFootstepCount) {

					//numFootstep = FMath::RandRange(0, footsteps.Num() - 1);
					numFootstep += 1;
					if (numFootstep >= footsteps.Num()) { numFootstep = 0; }
					//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(numFootstep));

					WalkSFX->Play();
					footstepcount = 0.0f;
				}
			}
		}

		// Breathing || stressBPM 100 max
		{
			HeartSFX->Sound = heartBeat[numHeartbeat];
			heartbeatCount += DeltaTime;

			actualHeartBeatCount = maxHeartbeatCount - 0.7f * (stressBPM / 100.0f);
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(actualHeartBeatCount));


			if (heartbeatCount >= actualHeartBeatCount) {
				numHeartbeat += 1;

				if (numHeartbeat >= heartBeat.Num()) { numHeartbeat = 0; }

				HeartSFX->Play();
				heartbeatCount = 0.0f;
			}
		}
	}
}


void AtestCharacter::MoveForward(float value) {
	AddMovementInput(GetActorForwardVector(), value);
	if (value == -1.0f || value == 1.0f) { isWalkingF = true;  }
	else { isWalkingF = false; }
}

void AtestCharacter::MoveSide(float value) {
	AddMovementInput(GetActorRightVector(), value);
	if (value == -1.0f || value == 1.0f) { isWalkingD = true; }
	else { isWalkingD = false; }
}

void AtestCharacter::MoveRunBegin() {
	//Code for run action
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed * runSpeed;
	isRuning = true;
}

void AtestCharacter::MoveRunEnd() {
	//Code for run action
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	isRuning = false;
}

void AtestCharacter::LookUp(float value) {
	CameraInput.Y += value * cameraSpeed;
	// Limit the rotation of the camera (not do a barrel roll)
	CameraInput.Y = FMath::Clamp(CameraInput.Y, -80.0f, 80.0f);
}

void AtestCharacter::LookSide(float value) {
	CameraInput.X += value * cameraSpeed;
	if (CameraInput.X < -360 || CameraInput.X > 360) { CameraInput.X = 0; }
}

void AtestCharacter::HideBegin() {
	//Code for begin hide action
	if (isGrabed) {
		//PhyHandle->ReleaseComponent();
		//isGrabed = false;
		UPrimitiveComponent* grabTemp = PhyHandle->GrabbedComponent;

		PhyHandle->ReleaseComponent();
		grabTemp->AddForce(FVector(Camera->GetForwardVector() * 1000.0 * 1000.0f * grabTemp->GetMass()));

		isGrabed = false;
	}
}

void AtestCharacter::HideEnd() {
	//Code for exit hide action
}

void AtestCharacter::GrabBegin() {
	Grabing();
}

void AtestCharacter::GrabEnd() {
	// Drop the item
	if (isGrabed) {
		//PhyHandle->ReleaseComponent();
		//isGrabed = false;
		UPrimitiveComponent* grabTemp = PhyHandle->GrabbedComponent;
		
		PhyHandle->ReleaseComponent();
		grabTemp->AddForce(FVector(Camera->GetForwardVector() * 1000.0f* grabTemp->GetMass()));
		
		isGrabed = false;
	}

}

void AtestCharacter::Grabing() {
	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	FCollisionResponseParams collisionResponse;

	collisionParams.AddIgnoredActor(this);

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("test clic")));


	// Cast a single Line trace face of the cam 
	if (gameWorld->LineTraceSingleByChannel(hitResult, Camera->GetComponentLocation(), Camera->GetForwardVector() * 500 + Camera->GetComponentLocation(), ECC_WorldStatic, collisionParams, collisionResponse))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("The Component Being Hit is: %s"), *hitResult.GetComponent()->GetName()));

		UPrimitiveComponent* ComponentToGrab = hitResult.GetComponent();

		PhyHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, hitResult.Location);

		if (*hitResult.GetComponent()->GetName() == FString("Cube") && PhyHandle->GrabbedComponent) {
			//PhyHandle->SetTargetLocation(hitResult.Location);
			isGrabed = true;
		}
	}
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
	PlayerInputComponent->BindAction("Hide", IE_Pressed, this, &AtestCharacter::HideBegin);
	PlayerInputComponent->BindAction("Hide", IE_Released, this, &AtestCharacter::HideEnd);

	// Grab
	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &AtestCharacter::GrabBegin);
	PlayerInputComponent->BindAction("Grab", IE_Released, this, &AtestCharacter::GrabEnd);
}

