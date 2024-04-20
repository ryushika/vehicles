

#include "Pawns/BasePawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Components/BrushComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/ShootingComponent.h"
#include "TimerManager.h"
#include "Tanks/TanksGameModeBase.h"
#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Components/Widgets/CrosshairWidgetComponent.h"
#include "UI/TurretCrosshairWidget.h"

ABasePawn::ABasePawn()
{
 	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Capsule Component"));
	SetRootComponent(BoxComponent);

	PawnMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pawn Mesh"));
	PawnMesh->SetupAttachment(GetRootComponent());
	//SetRootComponent(PawnMesh);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(PawnMesh, TEXT("CameraSocket"));
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	ShootPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Shoot Point"));
	ShootPoint->SetupAttachment(PawnMesh, TEXT("GunSocket"));

	ShootingComponent = CreateDefaultSubobject<UShootingComponent>(TEXT("Shooting Component"));
	AddOwnedComponent(ShootingComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	AddOwnedComponent(HealthComponent);

	TargetCrosshairComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Crosshair Widget Component"));
	TargetCrosshairComponent->SetupAttachment(GetRootComponent());

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Widget Component"));
	HealthWidgetComponent->SetupAttachment(GetRootComponent());

	TurretCrosshairComponent = CreateDefaultSubobject<UCrosshairWidgetComponent>(TEXT("Turret Widget Component"));
	TurretCrosshairComponent->SetupAttachment(GetRootComponent());

}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	InitializeComponents();
	
	if (GetController())
	{
		if (GetController()->IsPlayerController())
		{
			Tags.Add(TEXT("Player"));
		}
		else
		{
			Tags.Add(TEXT("Enemy"));
			BoxComponent->SetCollisionObjectType(ENEMY_CHANNEL);
		}
	}

	ShootPoint->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GunSocket"));
	TurretRotation = GetActorRotation();	
}

void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetController()->IsPlayerController())
	{
		TargetPoint = GetTargetPoint();
		LookAtTarget(TargetPoint + GetActorLocation());
	
		TurretTargetPoint = GetTurretTargetPoint();

		DrawTargetWidget();
		DrawTurretWidget();
	}
	else
	{
		DrawHealthWidget();
	}
}

void ABasePawn::InitializeComponents()
{
	if (ShootingComponent)
	{
		ShootingComponent->Initialize(this, ShootPoint);
		ShootingComponent->OnShoot.AddDynamic(this, &ABasePawn::InitiateReload);
	}
	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ABasePawn::OnDeath);
	}
}

void ABasePawn::DrawHealthWidget()
{
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		FRotator LookAtPlayerRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
		HealthWidgetComponent->SetWorldRotation(LookAtPlayerRotation);
	}
}

void ABasePawn::DrawTurretWidget()
{
	if (TurretCrosshairComponent)
	{
		FVector StartLocation = SpringArmComponent->GetComponentLocation();
		FVector LocalTurretLocation = TurretTargetPoint;
		if (FVector::Dist(StartLocation, LocalTurretLocation) >= DistanceToCrosshair)
		{
			LocalTurretLocation = StartLocation + (LocalTurretLocation - StartLocation).GetSafeNormal() * DistanceToCrosshair;
		}
		TurretCrosshairComponent->SetWorldLocation(LocalTurretLocation);
	}
}

void ABasePawn::DrawTargetWidget()
{
	if (TargetCrosshairComponent)
	{
		FVector StartLocation = SpringArmComponent->GetComponentLocation();
		FVector LocalTargetLocation = TargetPoint + GetActorLocation();
		if (FVector::Dist(StartLocation, LocalTargetLocation) >= DistanceToCrosshair)
		{
			LocalTargetLocation = StartLocation + (LocalTargetLocation - StartLocation).GetSafeNormal() * DistanceToCrosshair;
		}
		TargetCrosshairComponent->SetWorldLocation(LocalTargetLocation);
	}
}

FVector ABasePawn::GetTargetPoint()
{
	FVector StartLocation;
	FVector EndLocation;
	if (!bFreeLook)
	{
		FRotator Rotation;
		GetController()->GetPlayerViewPoint(StartLocation, Rotation);
		EndLocation = StartLocation + Rotation.Vector() * TraceDistance;
	}
	else
	{
		StartLocation = GetActorLocation() + FreeLookSavedLocation;
		EndLocation = StartLocation + (FreeLookSavedRotation.Vector() * TraceDistance);
	}
	return LineTrace(StartLocation, EndLocation) - GetActorLocation();
}

FVector ABasePawn::GetTurretTargetPoint()
{
	FVector TurretTraceStart = ShootPoint->GetComponentLocation();
	FVector TurretTraceEnd = TurretTraceStart + (ShootPoint->GetForwardVector() * TraceDistance);
	return LineTrace(TurretTraceStart, TurretTraceEnd);
}

void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePawn::Look);
	
		EnhancedInputComponent->BindAction(FreeLookAction, ETriggerEvent::Started, this, &ABasePawn::FreeLookPressed);
		EnhancedInputComponent->BindAction(FreeLookAction, ETriggerEvent::Completed, this, &ABasePawn::FreeLookPressed);

		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ABasePawn::Pause);
		
		EnhancedInputComponent->BindAction(MagnifyAction, ETriggerEvent::Started, this, &ABasePawn::Magnify);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, ShootingComponent.Get(), &UShootingComponent::StartShooting);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, ShootingComponent.Get(), &UShootingComponent::StopShooting);
	
		EnhancedInputComponent->BindAction(CycleModesAction, ETriggerEvent::Started, ShootingComponent.Get(), &UShootingComponent::CycleFireModes);
		EnhancedInputComponent->BindAction(CycleProjectilesAction, ETriggerEvent::Started, ShootingComponent.Get(), &UShootingComponent::CycleProjectileTypes);
	}
}

FVector ABasePawn::LineTrace(const FVector& StartLocation, const FVector& EndLocation) const
{
	if (GetController())
	{
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
		if (bHit)
		{
			return HitResult.Location;
		}
		else
		{
			return EndLocation;
		}
	}

	return FVector::ZeroVector;
}

void ABasePawn::InitiateReload(float ReloadTime)
{
	if (TurretCrosshairComponent && GetController()->IsPlayerController())
	{
		TurretCrosshairComponent->InitiateReload(ReloadTime);
	}
}

void ABasePawn::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerPitchInput(LookInput.Y);
		AddControllerYawInput(LookInput.X);
	}
}

void ABasePawn::LookAtTarget(const FVector& TargetLocation)
{
	if (ShootPoint)
	{
		FVector StartLocation = ShootPoint->GetComponentLocation();
		RotationToTarget = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation) - GetActorRotation();

		RotateTurret(RotationToTarget);
	}
}

void ABasePawn::RotateTurret(const FRotator& TargetRotation)
{
	FRotator NewRotation = FMath::RInterpConstantTo(TurretRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), GuidanceSpeed.X);

	float CurrentPitch = TurretRotation.Pitch;
	float TargetPitch = TargetRotation.Pitch;

	TargetPitch = FMath::Clamp(TargetPitch, MinVerticalGuidance, MaxVerticalGuidance);

	float NewPitch = FMath::FInterpConstantTo(CurrentPitch, TargetPitch, GetWorld()->GetDeltaSeconds(), GuidanceSpeed.Y);
	NewRotation.Pitch = NewPitch;

	TurretRotation = NewRotation;
}

void ABasePawn::Magnify(const FInputActionValue& Value)
{
	float FloatValue = Value.Get<float>();

	if (SpringArmComponent)
	{
		float ArmLength = SpringArmComponent->TargetArmLength;
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		float Magnification = FloatValue * MagnificationSpeed;
		SpringArmComponent->TargetArmLength = FMath::Clamp(ArmLength + Magnification, MaxMagnification, MinMagnification);
	}
}

void ABasePawn::FreeLookPressed(const FInputActionValue& Value)
{
	bFreeLook = Value.Get<bool>();
	if (bFreeLook)
	{
		GetController()->GetPlayerViewPoint(FreeLookSavedLocation, FreeLookSavedRotation);
		FreeLookSavedLocation = FreeLookSavedLocation - GetActorLocation();
	}
	else
	{
		GetController()->SetControlRotation(FreeLookSavedRotation);
		FreeLookSavedRotation = FRotator::ZeroRotator;
	}
}

void ABasePawn::Pause()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));	
	ATanksGameModeBase* GameMode = Cast<ATanksGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		if (GameMode->GetCurrentGameState() == EGameplayState::Playing)
		{
			GameMode->SetCurrentGameState(EGameplayState::Paused);
		}
		else if (GameMode->GetCurrentGameState() == EGameplayState::Paused)
		{
			GameMode->SetCurrentGameState(EGameplayState::Playing);
		}
	}
}

void ABasePawn::OnDeath()
{
	if (ATanksGameModeBase* GameMode = Cast<ATanksGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->PawnKilled(this);
	}
}
