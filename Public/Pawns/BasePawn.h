
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "BasePawn.generated.h"

class UHealthBarWidget;

UCLASS()
class TANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	ABasePawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//  Functions

public:
	// @returns Trace hit location or end location, if no hit
	FVector LineTrace(const FVector& StartLocation, const FVector& EndLocation) const;

	UFUNCTION(BlueprintPure)
	FORCEINLINE FRotator GetTurretRotation() const { return TurretRotation; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE class UHealthComponent* GetHealthComponent() const { return HealthComponent; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsLookingAtTarget(float Tolerance) const { return RotationToTarget.Equals(TurretRotation, Tolerance); };

	UFUNCTION()
	void InitiateReload(float ReloadTime);

protected:
	virtual void Look(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	virtual void LookAtTarget(const FVector& TargetLocation);
	virtual void RotateTurret(const FRotator& TargetRotation);

	void Magnify(const FInputActionValue& Value);
	void FreeLookPressed(const FInputActionValue& Value);

	void Pause();

	void InitializeComponents();
private:
	UFUNCTION()
	void OnDeath();

	FVector GetTurretTargetPoint();
	FVector GetTargetPoint();

	void DrawTurretWidget();
	void DrawTargetWidget();

	void DrawHealthWidget();

//  Variables

public:

protected:
	UPROPERTY(EditAnywhere, Category = "Pawn Behavior")
	float TraceDistance = 10000.f;

	UPROPERTY(EditAnywhere, Category = "Pawn Behavior")
	float DistanceToCrosshair = 1000.f;

	//In degrees/sec
	UPROPERTY(EditAnywhere, Category = "Pawn Behavior | Guidance")
	FVector2D GuidanceSpeed = FVector2D(20.f, 5.f);
	
	UPROPERTY(EditAnywhere, Category = "Pawn Behavior | Guidance")
	float MaxVerticalGuidance = 20.f;
	
	UPROPERTY(EditAnywhere, Category = "Pawn Behavior | Guidance")
	float MinVerticalGuidance = -10.f;

	UPROPERTY(EditAnywhere, Category = "Pawn Behavior | Magnification")
	float MagnificationSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Pawn Behavior | Magnification")
	float MaxMagnification = 0.f;

	UPROPERTY(EditAnywhere, Category = "Pawn Behavior | Magnification")
	float MinMagnification = 300.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USkeletalMeshComponent> PawnMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | Camera")
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | Shooting")
	TObjectPtr<class UShootingComponent> ShootingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | Shooting")
	TObjectPtr<class UArrowComponent> ShootPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | Health")
	TObjectPtr<class UHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | UI")
	TObjectPtr<class UWidgetComponent> HealthWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | UI")
	TObjectPtr<class UWidgetComponent> TargetCrosshairComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | UI")
	TObjectPtr<class UCrosshairWidgetComponent> TurretCrosshairComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | UI")
	UHealthBarWidget* HealthWidget;
	
	//In local space
	FVector TargetPoint;
	//in world space
	FVector TurretTargetPoint;

	bool bFreeLook = false;
	FRotator RotationToTarget;
	FRotator TurretRotation;
	FVector FreeLookSavedLocation;
	FRotator FreeLookSavedRotation;

private:
	// Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> FreeLookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MagnifyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ShootAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PauseAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CycleModesAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CycleProjectilesAction;
};
