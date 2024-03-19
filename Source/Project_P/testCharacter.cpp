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

	//camera 
	{
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
	}

	if (isShoked == true) {
		countShoked += DeltaTime;
		
		if(stressBPM <= 100) stressBPM += 0.3f;
		
		if (countShoked >= maxCountShoked) {
			isShoked = false;
			countShoked = 0.0f;
		}
	}

	// Stress jauge
	{
		if (ado != nullptr) {
			distance = GetDistanceTo(ado);
			//GEngine->AddOnScreenDebugMessage(-1, 0.10f, FColor::Blue, FString::SanitizeFloat(distance));
		
			if (distance <= maxDistanceAdo && stressBPM <= 100) {
				stressBPM += 0.5;
			}
			/*if (distance >= maxDistanceAdo && stressBPM >= 0) {
				stressBPM -= 0.5;
			}*/
		
		}
		
		if (stressBPM >= 0)stressBPM -= 0.01;

		if (isRuning == true) {
			countHide += DeltaTime;
			if (countHide >= maxCountHide) {
				stressBPM += 0.05f;
			}
		}

		if (isHide == true) {
			countRuning += DeltaTime;
			if (countRuning >= maxCountRuning) {
				stressBPM += 0.08f;
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
	countRuning = 0.0f;

}

void AtestCharacter::LookUp(float value) {
	if (isShoked == false) CameraInput.Y += value * cameraSpeed;
	// Limit the rotation of the camera (not do a barrel roll)
	CameraInput.Y = FMath::Clamp(CameraInput.Y, -80.0f, 80.0f);
}

void AtestCharacter::LookSide(float value) {
	if (isShoked == false) CameraInput.X += value * cameraSpeed;
	if (CameraInput.X < -360 || CameraInput.X > 360) { CameraInput.X = 0; }
}

void AtestCharacter::HideBegin() {
	//Code for begin hide action
	if (isGrabed) {
		//PhyHandle->ReleaseComponent();
		//isGrabed = false;
		UPrimitiveComponent* grabTemp = PhyHandle->GrabbedComponent;

		PhyHandle->ReleaseComponent();
		grabTemp->AddForce(FVector(Camera->GetForwardVector() * YeetForce * 1000.0f * grabTemp->GetMass()));

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
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
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

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Grab clic")));


	// Cast a single Line trace face of the cam 
	if (gameWorld->LineTraceSingleByChannel(hitResult, Camera->GetComponentLocation(), Camera->GetForwardVector() * 500 + Camera->GetComponentLocation(), ECC_WorldStatic, collisionParams, collisionResponse))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("The Component Being Hit is: %s"), *hitResult.GetComponent()->GetName()));

		UPrimitiveComponent* ComponentToGrab = hitResult.GetComponent();

		PhyHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, hitResult.Location);

		//Grab item
		if (*hitResult.GetComponent()->GetName() == FString("Cube") ||
			*hitResult.GetComponent()->GetName() == FString("Sac") &&
			PhyHandle->GrabbedComponent) {
			GetCharacterMovement()->MaxWalkSpeed = walkSpeed/2;
			//PhyHandle->SetTargetLocation(hitResult.Location);
			isGrabed = true;
		}

		//Hide item
		if (*hitResult.GetComponent()->GetName() == FString("Table")){

		}
	}
}

void AtestCharacter::Shoked(FRotator childRotation) {

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, childRotation.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, this->GetActorRotation().ToString());

	FRotator newLook = childRotation - FRotator(0, 180, 0);


	CameraInput.X = newLook.Yaw;

	SetActorRotation(newLook);

	FRotator NewRotationX = GetActorRotation();
	NewRotationX.Yaw = CameraInput.X;
	SetActorRotation(NewRotationX);

	isShoked = true;
}

void AtestCharacter::changeBPM(bool add, float number) {
	if (add == true) {
		stressBPM += number;
		if (stressBPM >= 100.0f) { stressBPM = 100.0f; }
	}
	else {
		stressBPM -= number;
		if (stressBPM <= 0.0f) { stressBPM = 0.0f; }
	}
}

bool AtestCharacter::getIsHide() { return isHide; }

void AtestCharacter::setIsHide(bool hide) { isHide = hide; }

AActor* AtestCharacter::hideChar(AActor* choseOne) {
	UPrimitiveComponent* hideComponent;

	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	FCollisionResponseParams collisionResponse;

	collisionParams.AddIgnoredActor(this);

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Hide clic")));


	// Cast a single Line trace face of the cam 
	if (gameWorld->LineTraceSingleByChannel(hitResult, Camera->GetComponentLocation(), Camera->GetForwardVector() * 500 + Camera->GetComponentLocation(), ECC_WorldStatic, collisionParams, collisionResponse))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("The Component Being Hit is: %s"), *hitResult.GetComponent()->GetName()));

		hideComponent = hitResult.GetComponent();

		//PhyHandle->GrabComponentAtLocation(hideComponent, NAME_None, hitResult.Location);

		//Hide item
		if (*hitResult.GetComponent()->GetName() == FString("Table") || 
			*hitResult.GetComponent()->GetName() == FString("porte1") ||
			*hitResult.GetComponent()->GetName() == FString("porte2")) {

			return hitResult.GetActor();
		}
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, FString::Printf(TEXT("Not a waiting component as ben hit !")));
	return nullptr;
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

