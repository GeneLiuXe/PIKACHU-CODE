typedef long long ll;

ll gcd(ll a,ll b)
{
	return b == 0 ? a:gcd(b,a%b);
}