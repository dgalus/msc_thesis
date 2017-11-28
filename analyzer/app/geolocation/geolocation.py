import GeoIP

class IP_Country:
    @staticmethod
    def get_country_by_name(name):
        gi = GeoIP.open("./GeoIP.dat", GeoIP.GEOIP_STANDARD)
        return gi.country_code_by_name(name)

    @staticmethod
    def get_country_by_address(address):
        gi = GeoIP.open("./GeoIP.dat", GeoIP.GEOIP_STANDARD)
        return gi.country_code_by_addr(address)


#print(gi.country_code_by_name("yahoo.de"))
#print(gi.country_name_by_name("bundestag.de"))
#print(gi.country_code_by_addr("8.8.8.8"))
#print(gi.country_name_by_addr("8.8.8.8"))