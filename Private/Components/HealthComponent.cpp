

#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (AActor* LocalOwner = GetOwner())
	{
		LocalOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}

	Health = MaxHealth;
}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HealthTimeline.TickTimeline(DeltaTime);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UpdateHealth(-Damage);
	LOG(TEXT("Health: %f"), Health);

}

FText UHealthComponent::GetHealthIntText()
{
	int32 HP = FMath::RoundHalfFromZero(GetHealthPercentage() * 100);
	FString HealthHUD =  FString::FromInt(HP) + FString(TEXT("%"));
	FText HPText = FText::FromString(HealthHUD);
	return HPText;
}


void UHealthComponent::UpdateHealth(float DeltaHealth)
{
	Health = FMath::Clamp(Health + DeltaHealth, 0.f, MaxHealth);
	OnDamageTaken.Broadcast(DeltaHealth);
	if (Health <= 1.f)
	{
		OnDeath.Broadcast();
	}
}

