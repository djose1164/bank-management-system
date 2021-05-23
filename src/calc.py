"""calc module where are contain the functions for convertion between the
several divisas that the bank supports.

This module can convert between USD, EUR and DOP. From and to each one.
"""
from typing import Text
import requests
from requests.models import Response

digits: int = 2

# TODO: Fix request to API error.

def _do_connection_to_api(_from: str, to: str):
    BASE_URL: str = "https://free.currconv.com/api/v7/"
    API_KEY: str = "&apiKey=25ddb3e387f8c1e7611e"
    request: str = "convert"
    base: str = f"?q={_from}_{to}"
    symbols: str = "&compact=ultra"
    request_url: str = BASE_URL + request + base + symbols + API_KEY
    return requests.get(request_url)


def get_eur_to_usd():
    reponse = _do_connection_to_api("EUR", "USD")
    return reponse.json()["EUR_USD"]


def get_usd_to_eur():
    reponse = _do_connection_to_api("USD", "EUR")
    return reponse.json()["USD_EUR"]


def get_dop_to_usd():
    response = _do_connection_to_api("DOP", "USD")
    return response.json()["DOP_USD"]


def get_dop_to_eur():
    response = _do_connection_to_api("DOP", "EUR")
    return response.json()["DOP_EUR"]

def get_eur_to_dop():
    response = _do_connection_to_api("EUR", "DOP")
    return response.json()["EUR_DOP"]

def get_usd_to_dop():
    response = _do_connection_to_api("USD", "DOP")
    return response.json()["USD_DOP"]

def dollars_to_euros(amount: float):
    converted_value: float = amount * get_usd_to_eur()
    return round(converted_value, digits)


def euros_to_dollars(amount: float):
    converted_value: float = amount * get_eur_to_usd()
    return round(converted_value, digits)


def dop_to_dollars(amount: float):
    converted_value: float = amount * get_dop_to_usd()
    return round(converted_value, digits)


def dop_to_euros(amount: float):
    converted_value: float = amount * get_dop_to_eur()
    return round(converted_value, digits)

def dollars_to_dop(amount: float):
    converted_value: float = amount * get_usd_to_dop()
    return round(converted_value, digits)

def euros_to_dop(amount: float):
    converted_value: float = amount * get_eur_to_dop()
    return round(converted_value, digits)

